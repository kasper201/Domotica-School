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
    char temp[MESSAGE_SIZE];
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
        extractStrings(Message, first, second);
        printk("Nieuwschierig %s %s \n", first, second);
        strcpy(temp, second);
        extractStrings(temp, first, second);
        if(strstr(first, "STM32"))
        {
            strcpy(temp, second);
            extractStrings(temp, first, second);
            if(strstr(first, "STM_LED"))
            {
                strcpy(temp, second);
                extractStrings(temp, first, second);
                if(strstr(first, "true"))
                {
                    *ledState = true;
                    printk("UpdateAppActuator STM32 STM_LED true");
                }
                if(strstr(first, "false"))
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
    char delimiter = ' ';

    int currentCounter = 0;

    while(input[currentCounter] != delimiter)
    {
        first[currentCounter] = input[currentCounter];
        currentCounter++;
    }

    currentCounter++;

    int secondCounter = 0;
    while(currentCounter < MESSAGE_SIZE)
    {
        second[secondCounter] = input[currentCounter];
        currentCounter++;
        secondCounter++;
    }
}