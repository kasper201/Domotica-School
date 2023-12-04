#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include "bluetooth.h"
#include "board.h"

void main(void)
{
    init();
    ledSet(true);
    k_msleep(500);
    ledSet(false);
    int i = 1;
    while(1)
    {

    }
}
