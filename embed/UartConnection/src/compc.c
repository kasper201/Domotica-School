#include "compc.h"
#include "uart.h"
#include "nodeData.h"

#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <string.h>

#define MESSAGE_SIZE 128 //Defines the maximum size for the incoming string

//Reads input from the application and decides what to do with it
void readPc(char (*nodes)[MAX_NODE_INFO_STRING_LENGTH], bool *ledState)
{
    char Message[MESSAGE_SIZE];
    k_msgq_get(&uart_msgq, &Message, K_NO_WAIT);
    k_msgq_cleanup(&uart_msgq);

    if (strstr(Message, "connected"))
    {
        printk("Connection is established\n");
        k_msleep(5);
        nodesToPc(nodes);
    }

    if (strstr(Message, "AddGroup"))
    {
        printk("Group will be added\n");
    }

    if (strstr(Message, "UpdateActuator"))
    {
        char *token = strtok(Message, " ");
        token = strtok(NULL, " ");
        if(strstr(token, "STM32"))
        {
            token = strtok(NULL, " ");
            if(strstr(token, "STM_LED"))
            {
                token = strtok(NULL, " ");
                if(strstr(token, "true"))
                {
                    *ledState = true;
                    printk("UpdateAppActuator STM32 STM_LED true");
                }
                if(strstr(token, "false"))
                {
                    *ledState = false;
                    printk("UpdateAppActuator STM32 STM_LED false");
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