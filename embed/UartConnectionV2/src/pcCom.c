#include "uart.h"
#include "bluetooth.h"
#include "pcCom.h"
#include "board.h"
#include "group.h"	// Contains the subscribeToGroup functions etc.

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MESSAGE_SIZE 256 // Defines the maximum size for the incoming string

#define MAX_NAMES 256
#define MAX_NAME_LENGTH 13

static char names[MAX_NAMES][MAX_NAME_LENGTH];
static int nameCount = 0;
static uint16_t groupAddresses[MAX_NAMES];

static int mr = 0;

//readPC functions
void caseConnected(char *Message)
{
    char onoff[] = {"off"};
    if(onoffVal())
    {
        strcpy(onoff, "on");
    }
    printk("Connection established\n");
    printk("AddNode PcNode %05d %05d AddSensor Button pcButton AddActuator LED pcLED %s", getAddr(), getElem(0), onoff);
    for (int i = 0; i < nameCount; i++)
    {
        printk("AddGroup %d %s\n", groupAddresses[i], names[i]); // Group 49152 [groupname]
    }
}

void deleteGroup(char *Message)
{
    int index_to_delete = -1;

    // Find the index of the group to delete
    for (int i = 0; i < nameCount; i++) {
        if (groupAddresses[i] == atoi(Message)) {
            index_to_delete = i;
            break;
        }
    }

    // If the group was not found, return
    if (index_to_delete == -1) {
        return;
    }

    // Shift all elements after the index to delete one position to the left
    for (int i = index_to_delete; i < nameCount - 1; i++) {
        groupAddresses[i] = groupAddresses[i + 1];
        strcpy(names[i], names[i + 1]);
    }

    // Decrease the count of names
    nameCount--;
}

void createGroupFinal(char *Message, int i)
{
    groupAddresses[i] = atoi(Message); // address of the group that is being created
    memmove(Message, Message + 6, strlen(Message) - 6 + 1); //removes create_group_ from message
    strcpy(names[i], Message);
}

void createGroup(char *Message)
{
    for(int i = 0; i < nameCount; i++)
    {
        if(groupAddresses[i] == atoi(Message) && strstr(Message, names[i]))
        {
            printk("Group already exists\n");
            return;
        }
        else if(groupAddresses[i] == atoi(Message) && !strstr(Message, names[i]))
        {
            createGroupFinal(Message, i);
            return;
        }
    }
    createGroupFinal(Message, nameCount);
    nameCount++;
}

void groupToggle(char *Message)
{
    uint16_t groupAddress = atoi(Message);

    printk("Toggling group %04x\n", groupAddress);
    gen_onoff_send(!onoffVal(), groupAddress); //send onoff message to group
}

void uartSubscribeGroup(char *Message, bool sub)
{
    printk("Message: %s\n", Message);
    uint16_t netKeyIndex = 0x0002;
    uint16_t groupAddress = atoi(Message); // address of the group that is being subscribed to
    memmove(Message, Message + 6, strlen(Message) - 6 + 1);
    uint16_t elementAddress = atoi(Message); // element address of the device that is subscribing to the group
    memmove(Message, Message + 6, strlen(Message) - 6 + 1);
    uint16_t mod_id = atoi(Message); // model id of the device that is subscribing to the group
    memmove(Message, Message + 6, strlen(Message) - 6 + 1);
    uint16_t address = atoi(Message); // address of the device that is subscribing to the group

    printk("Subscribing to group %04x\n", groupAddress);
    getNetIdx(&netKeyIndex);
    k_msleep(1); // wait for netKeyIndex to be set
    printk("netKeyIndex: %04x\n", netKeyIndex);	
    printk("index: 0x%04x, address: 0x%04x, elementAddress: 0x%04x, groupAddress: 0x%04x, mod_id: 0x%04x\n", netKeyIndex, address, elementAddress, groupAddress, mod_id);
    if(sub)
        subscribeToGroup(groupAddress, elementAddress, mod_id, netKeyIndex, address);
    else
        unsubscribeFromGroup(groupAddress, elementAddress, mod_id, netKeyIndex, address);
}

// Reads input from the application and decides what to do with it
void readPc()
{
    char Message[MESSAGE_SIZE];

    // Clear the message array
    for (int i = 0; i < MESSAGE_SIZE; i++)
    {
        Message[i] = '\0';
    }

    k_msgq_get(&uart_msgq, &Message, K_NO_WAIT);
    k_msgq_cleanup(&uart_msgq);

    // Send out already existing groups
    if (strstr(Message, "connected"))
    {
        caseConnected(Message);
        mr = 1;
    } 
    else if(strstr(Message, "toggle_group_")) // checks for "toggle_group" in the message	
    {
        memmove(Message, Message + 13 + mr, strlen(Message) - 13 + mr + 1); //removes toggle_group_ from message
        groupToggle(Message);
        mr = 1;
    }
    else if(strstr(Message, "unsubscribe_group_"))
    {
        memmove(Message, Message + 18 + mr, strlen(Message) - 18 + mr + 1); //removes unsubscribe_group_ from message
        // printk("Message: %s\n", Message);
        uartSubscribeGroup(Message, false);
        mr = 1;
    }
    else if(strstr(Message, "subscribe_group_"))
    {
        memmove(Message, Message + 16+ mr, strlen(Message) - 16 + mr + 1); //removes subscribe_group_ from message
        // printk("Message: %s\n", Message);
        uartSubscribeGroup(Message, true);
        mr = 1;
    }
    else if(strstr(Message, "create_group_"))
    {
        memmove(Message, Message + 13 + mr, strlen(Message) - 13 + mr + 1); //removes create_group_ from message
        createGroup(Message);
        mr = 1;
    }
    else if(strstr(Message, "delete_group_"))
    {
        memmove(Message, Message + 13 + mr, strlen(Message) - 13 + mr + 1); //removes delete_group_ from message
        deleteGroup(Message);
        mr = 1;
    }
    else if(strlen(Message) > 0) // if the message is not empty and does not contain any of the above defined commands
    {
        printk("Unknown command\n");
        mr = 1;
    }
    k_msgq_cleanup(&uart_msgq);
}