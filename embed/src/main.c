#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include "bluetooth.h"
#include "board.h"

void init()
{
    ledInit();
}

void main(void)
{
    init();
    ledSet(true);
}
