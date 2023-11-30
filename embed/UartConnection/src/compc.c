#include "compc.h"
#include "uart.h"

#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#define MESSAGE_SIZE 128 //Defines the maximum size for the incoming string

void readPc(void)
{
    char Message[MESSAGE_SIZE];
    k_msgq_get(&uart_msgq, &Message, K_NO_WAIT);
    k_msgq_cleanup(&uart_msgq);

    if (strstr(Message, "connected"))
    {
        printk("Connection is established\n");
        k_msleep(5);
        printk("AddNode STM32 AddSensor Button STM_Button AddActuator LED STM_LED false\n");
    }

    if (strstr(Message, "AddGroup"))
    {
        printk("Group will be added\n");
    }
    
    for(int i = 0; i < MESSAGE_SIZE; i++)
    {
        Message[i] = ' ';
    }
    k_msgq_cleanup(&uart_msgq);
}