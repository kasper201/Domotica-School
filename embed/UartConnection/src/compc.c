#include "compc.h"
#include "uart.h"
#include "nodeData.h"
#include "groupData.h"
#include "main.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <string.h>

#define MESSAGE_SIZE 200 //Defines the maximum size for the incoming string

//Reads input from the application and decides what to do with it
void readPc(struct Node* node, struct Group* group, int *ledState)
{
    char Message[MESSAGE_SIZE];
    k_msgq_get(&uart_msgq, &Message, K_NO_WAIT);
    k_msgq_cleanup(&uart_msgq);

    //Send out already existing groups
    if (strstr(Message, "connected"))
    {
        printk("Connection is established\n");
        k_msleep(3);
        nodesToPc(node);
        k_msleep(3);
        groupsToPc(group);
        //empties Message
        for(int i = 0; i < MESSAGE_SIZE; i++)
        {
            Message[i] = '0';
        }
    }

    //Create a group
    if (strstr(Message, "CreateGroup"))
    {
        createGroup(group, Message);
    }

    //Update a group
    if (strstr(Message, "UpdateGroup"))
    {
        updateGroup(group, Message);
    }

    //Change led status
    if (strstr(Message, "UpdateActuator"))
    {
        if(strstr(Message, "STM32_______"))
        {
            if(strstr(Message, "STM_LED_____"))
            {
                if(strstr(Message, "true"))
                {
                    *ledState = 1;
                    printk("UpdateAppActuator STM32_______ STM_LED_____ true\n");
                }
                if(strstr(Message, "false"))
                {
                    *ledState = 0;
                    printk("UpdateAppActuator STM32_______ STM_LED_____ false\n");
                }
            }
        }
    }
    
    for(int i = 0; i < MESSAGE_SIZE; i++)
    {
        Message[i] = ' ';
    }
    k_msgq_cleanup(&uart_msgq);
}