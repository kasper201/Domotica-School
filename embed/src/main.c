#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include "bluetooth.h"

static struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
static struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0});

void led1And2()
{
    k_msleep(1000);
    printk("booting\n");
    if (!gpio_is_ready_dt(&led0))
    {
        printk("Error: LED device %s is not ready\n", led0.port->name);
        return;
    }

    gpio_pin_configure_dt(&led0, GPIO_OUTPUT);
    gpio_pin_configure_dt(&led1, GPIO_OUTPUT);

    gpio_pin_set_dt(&led0, 1);
    k_msleep(1000);
    int led_state = gpio_pin_get_dt(&led0);
    printk("LED0 state: %d\n", led_state);
    k_msleep(1000);
    gpio_pin_set_dt(&led0, 0);
    gpio_pin_set_dt(&led1, 0);
}

void main(void)
{
    led1And2();
}
