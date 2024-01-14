#include "uart.h"
#include "bluetooth.h"
#include "pcCom.h"
#include "board.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <string.h>

#define MESSAGE_SIZE 256 // Defines the maximum size for the incoming string

void caseConnected(char *Message)
{
    printk("Connection is established\n");
    k_msleep(3);
    // empties Message
    for (int i = 0; i < MESSAGE_SIZE; i++)
    {
        Message[i] = '0';
    }
}

void groupToggle(bool onoff, char *Message)
{
    if(onoff)
        memmove(Message, Message + 16, strlen(Message) - 16 + 1);
    else
        memmove(Message, Message + 17, strlen(Message) - 17 + 1);
    uint16_t groupAddress = atoi(Message);

    printk("Toggling group %d\n", groupAddress);
    gen_onoff_send(onoff, groupAddress); //send onoff message to group

}

// Reads input from the application and decides what to do with it
void readPc()
{
    char Message[MESSAGE_SIZE];
    k_msgq_get(&uart_msgq, &Message, K_NO_WAIT);
    k_msgq_cleanup(&uart_msgq);

    // Send out already existing groups
    if (strstr(Message, "connected"))
    {
        caseConnected(Message);
    } 
    else if(strstr(Message, "toggle_group_")) // checks for "toggle_group_on" or "toggle_group_off"
    {
        if(strstr(Message, "on_"))
        {
            groupToggle(true, Message);
        }
        else if(strstr(Message, "off_"))
        {
            groupToggle(false, Message);
        }
    }

    for (int i = 0; i < MESSAGE_SIZE; i++)
    {
        Message[i] = ' ';
    }
    k_msgq_cleanup(&uart_msgq);
}