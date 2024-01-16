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

static int mr = 0;

//readPC functions
void caseConnected(char *Message)
{
    printk("Connection established\n");
    // Send out already existing groups (if possible)

}

void groupToggle(char *Message)
{
    uint16_t groupAddress = atoi(Message);

    printk("Toggling group %04x\n", groupAddress);
    gen_onoff_send(!onoffVal(), groupAddress); //send onoff message to group
}

void uartSubscribeGroup(char *Message)
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
    //getNetIdx(&netKeyIndex);
    k_msleep(1); // wait for netKeyIndex to be set
    printk("netKeyIndex: %04x\n", netKeyIndex);	
    printk("index: 0x%04x, address: 0x%04x, elementAddress: 0x%04x, groupAddress: 0x%04x, mod_id: 0x%04x\n", netKeyIndex, address, elementAddress, groupAddress, mod_id);
    subscribeToGroup(netKeyIndex, address, elementAddress, groupAddress, mod_id);
}

void uartUnsubscribeGroup(char *Message)
{
    uint16_t netKeyIndex = 0x0000;
    uint16_t groupAddress = atoi(Message); // address of the group that is being subscribed to
    memmove(Message, Message + 6, strlen(Message) - 6 + 1);
    uint16_t elementAddress = atoi(Message); // element address of the device that is subscribing to the group
    memmove(Message, Message + 6, strlen(Message) - 6 + 1);
    uint16_t mod_id = atoi(Message); // model id of the device that is subscribing to the group
    memmove(Message, Message + 6, strlen(Message) - 6 + 1);
    uint16_t address = atoi(Message); // address of the device that is subscribing to the group

    printk("Unsubscribing from group %04x\n", groupAddress);
    getNetIdx(&netKeyIndex);
    k_msleep(1); // wait for netKeyIndex to be set
    printk("netKeyIndex: %04x\n", netKeyIndex);	
    printk("index: 0x%04x, address: 0x%04x, elementAddress: 0x%04x, groupAddress: 0x%04x, mod_id: 0x%04x\n", netKeyIndex, address, elementAddress, groupAddress, mod_id);
    unsubscribeFromGroup(netKeyIndex, address, elementAddress, groupAddress, mod_id);
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
        memmove(Message, Message + 14 + mr, strlen(Message) - 14 + mr + 1);
        groupToggle(Message);
        mr = 1;

    }
    else if(strstr(Message, "unsubscribe_group_"))
    {
        memmove(Message, Message + 18 + mr, strlen(Message) - 18 + mr + 1);
        // printk("Message: %s\n", Message);
        uartUnsubscribeGroup(Message);
        mr = 1;
    }
    else if(strstr(Message, "subscribe_group_"))
    {
        memmove(Message, Message + 16+ mr, strlen(Message) - 16 + mr + 1);
        // printk("Message: %s\n", Message);
        uartSubscribeGroup(Message);
        mr = 1;
    }
    else if(strlen(Message) > 0)
    {
        printk("Unknown command\n");
    }

    k_msgq_cleanup(&uart_msgq);
}