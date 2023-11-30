#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include "uart.h"

//Setup uart connection to pc
#define UART_DEVICE_NODE DT_ALIAS(usart)
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

int uartSetup()
{
	if (!device_is_ready(uart_dev))
	{
		printk("UART device not found!");
		return -1;
	} else
    {
        printk("UART device found!");
    }

	/* configure interrupt and callback to receive data */
    /*
	int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

	if (ret < 0)
	{
		if (ret == -ENOTSUP)
		{
			printk("Interrupt-driven UART API support not enabled\n");
		}
		else if (ret == -ENOSYS)
		{
			printk("UART device does not support interrupt-driven API\n");
		}
		else
		{
			printk("Error setting UART callback: %d\n", ret);
		}
		return -1;
	}*/

	uart_irq_rx_enable(uart_dev);
	return 0;
}