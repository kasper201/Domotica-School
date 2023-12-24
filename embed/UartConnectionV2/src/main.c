#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include "bluetooth.h"
#include "board.h"
#include "uart.h"

// Values for setup uartReceive
k_thread_stack_define(uart_thread_stack, 1024);
struct k_thread uart_thread_data;

void uart_thread_function(void *a, void *b, void *c)
{
    while (1)
    {
        char buf[128];
        int len = k_msgq_get(&uart_msgq, buf, K_FOREVER);
        buf[len] = '\0';
        printk("Received: %s\n", buf);
    }
}

int main(void)
{
    init();
    while(1)
    {
        /*ledSet(true);
        k_msleep(500);
        ledSet(false);
        k_msleep(500);*/
    }
    return 0;
}
