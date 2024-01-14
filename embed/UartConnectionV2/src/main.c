#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>

#include "pcCom.h"
#include "bluetooth.h"
#include "board.h"
#include "uart.h"
#include "group.h"

// // Values for setup uartReceive
// K_THREAD_STACK_DEFINE(uart_thread_stack, 1024);
// struct k_thread uart_thread_data;

void uart()
{
    printf("UART thread started\n");
    while (1)
    {
        //char buf[128];
        //int len = k_msgq_get(&uart_msgq, buf, K_FOREVER);
        //buf[len] = '\0';
        // if(len != 0)
        // {
        //     if(buf[0] == 's')
        //     {
        //         printk("Subscribing to group\n");
        //         uint16_t groupAddress = 0x0001;
        //         uint16_t elementAddress = 0x0001;
        //         uint16_t mod_id = 0x1000;
        //         uint16_t netKeyIndex = 0x000;
        //         uint16_t address = 0x0001;
        //         subscribeToGroup(groupAddress, elementAddress, mod_id, netKeyIndex, address);
        //     }
        //     //publish to group
        //     /*else if(buf[0] == 'p')
        //     {
        //         //printk("Publishing to group\n");
        //         uint16_t groupAddress = 0x0001;
        //         uint16_t elementAddress = 0x0001;
        //         uint16_t mod_id = 0x1000;
        //         uint16_t netKeyIndex = 0x000;
        //         uint16_t address = 0x0001;
        //         publishToGroup(groupAddress, elementAddress, mod_id, netKeyIndex, address);
        //     }*//*
        //     else if(buf[0] == 'u')
        //     {
        //         //printk("Unsubscribing from group\n");
        //         uint16_t groupAddress = 0x0001;
        //         uint16_t elementAddress = 0x0001;
        //         uint16_t mod_id = 0x1000;
        //         uint16_t netKeyIndex = 0x000;
        //         uint16_t address = 0x0001;
        //         unsubscribeFromGroup(groupAddress, elementAddress, mod_id, netKeyIndex, address);
        //     }
        //     else if(buf[0] == 'c')
        //     {
        //         //printk("Clearing group\n");
        //         uint16_t groupAddress = 0x0001;
        //         uint16_t elementAddress = 0x0001;
        //         uint16_t mod_id = 0x1000;
        //         uint16_t netKeyIndex = 0x000;
        //         uint16_t address = 0x0001;
        //         clearGroup(groupAddress, elementAddress, mod_id, netKeyIndex, address);
        //     }
        //     else if(buf[0] == 'r')
        //     {
        //         //printk("Resetting node\n");
        //         resetNode();
        //     }
        //     else if(buf[0] == 'a')
        //     {
        //         ///added to group
        //     }
        //     else if(buf[0] == 'd')
        //     {
        //         ///disconnected from group
        //     }*/
        // }
    }
}

int main(void)
{
    init();
    k_msleep(1000);
    printk("Starting\n");
    // k_thread_create(&uart_thread_data, uart_thread_stack, K_THREAD_STACK_SIZEOF(uart_thread_stack), uart_thread_function, NULL, NULL, NULL, 0, K_INHERIT_PERMS, K_NO_WAIT); // Create thread for uart
    while(1)
    {
        // ledSet(true);
        // k_msleep(500);
        // ledSet(false);
        // k_msleep(500);
        readPc();
        //uart();
    }
    return 0;
}

// west build -p always -b nrf52833dk_nrf52833 D:\domotica\DOMOTICA-SCHOOL\embed\UartConnectionV2\