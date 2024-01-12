#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <string.h>

#include "uart.h"
#include "bluetooth.h"
#include "pcCom.h"

#define MESSAGE_SIZE 128

//Setup uart connection to pc
int readPC()
{
    char Message[MESSAGE_SIZE + 1]; // +1 for the null terminator
    int ret;

    ret = k_msgq_get(&uart_msgq, &Message, K_NO_WAIT);
    if (ret != 0) {
        // Handle the error
        return 0;
    }

    Message[MESSAGE_SIZE] = '\0'; // Null-terminate Message

    if(strstr(Message, "test123"))
    {
        printk("Message received\n");
        return 1;
    }
    printk("%s\n", Message);
    return 0;
}