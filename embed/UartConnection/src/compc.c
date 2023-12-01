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

#define MESSAGE_SIZE 200 //Defines the maximum size for the incoming string

//Reads input from the application and decides what to do with it
void readPc(char (*nodes)[MAX_NODE_INFO_STRING_LENGTH], bool *ledState)
{
    char Message[MESSAGE_SIZE];
    char first[MESSAGE_SIZE];
    char second[MESSAGE_SIZE];
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
        extracStrings(Message, *first, *second);
        printk("Nieuwschierig %s %s \n", first, second);
        char *token = strtok(Message, " ");
        token = strtok(NULL, " ");
        if(strstr(token, "STM32"))
        {
            token = strtok(NULL, " ");
            printk("check2 %s\n", token);
            if(strstr(token, "STM_LED"))
            {
                printk("check3\n");
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

//Gets word before first white space
void extractStrings(const char *input, char *first, char *second) {
    char *delimiter = ' ';
    char *token = strtok((char *)input, delimiter);

    if (token != NULL) {
        // Copy the first part to the 'first' string
        strncpy(first, token, strlen(token));
        first[strlen(token)] = '\0';

        // Get the next token (if any)
        token = strtok(NULL, delimiter);

        if (token != NULL) {
            // Copy the second part to the 'second' string
            strncpy(second, token, strlen(token));
            second[strlen(token)] = '\0';
        }
    }
}