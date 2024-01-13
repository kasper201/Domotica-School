#include "uart.h"
//#include "bluetooth.h"
#include "pcCom.h"
#include "board.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <string.h>

#define MESSAGE_SIZE 200 // Defines the maximum size for the incoming string

// Reads input from the application and decides what to do with it
void readPc()
{
    char Message[MESSAGE_SIZE];
    k_msgq_get(&uart_msgq, &Message, K_NO_WAIT);
    k_msgq_cleanup(&uart_msgq);

    // Send out already existing groups
    if (strstr(Message, "connect"))
    {
        printk("Connection is established\n");
        ledSet(true);
        // empties Message
        for (int i = 0; i < MESSAGE_SIZE; i++)
        {
            Message[i] = '0';
        }
    }

    // Create a group
    if (strstr(Message, "CreateGroup"))
    {
        printk("Creating group\n");
        
    }

    for (int i = 0; i < MESSAGE_SIZE; i++)
    {
        Message[i] = ' ';
    }
    k_msgq_cleanup(&uart_msgq);
}