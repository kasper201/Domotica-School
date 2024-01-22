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

int main(void)
{
    init();
    k_msleep(1000);
    printk("Starting\n");
    while(1)
    {
        provMain();
        readPc();
    }
    return 0;
}

// west build -p always -b nrf52833dk_nrf52833 D:\domotica\DOMOTICA-SCHOOL\embed\UartConnectionV2