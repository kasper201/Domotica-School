//#include <zephyr/bluetooth/mesh.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#include "board.h"

// determine if either sw0 or button0 exists in the device tree
#if DT_NODE_EXISTS(sw0)
#define BUTTON0	DT_ALIAS(sw0)
#elif DT_NODE_EXISTS(button0) 
#define BUTTON0 DT_ALIAS(button0)
#endif 

#if DT_NODE_EXISTS(BUTTON0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(BUTTON0, gpios,
							      {0});
static struct gpio_callback button_cb_data;
#endif /*button0*/

static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});


void ledInit() // most can be removed after testing
{
    k_msleep(1000);
    if (!gpio_is_ready_dt(&led))
    {
        printk("Error: LED device %s is not ready\n", led.port->name);
        return;
    }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT);
    gpio_pin_set_dt(&led, 1);
    k_msleep(100);
    gpio_pin_set_dt(&led, 0);
}

void buttonInit()
{
    
}

void init()
{
    ledInit();
    buttonInit();
}

void ledSet(bool value)
{
    gpio_pin_set_dt(&led, value);
}