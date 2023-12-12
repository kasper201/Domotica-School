#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include "bluetooth.h"
#include "board.h"

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
