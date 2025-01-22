/*
 * Button code is highly based on samples\basic\button
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>
#include <inttypes.h>

#include "board.h"
#include "bluetooth.h"

#if DT_NODE_EXISTS(DT_ALIAS(sw0))
#define BUTTON0_NODE DT_ALIAS(sw0)
#elif DT_NODE_EXISTS(DT_ALIAS(button0))
#define BUTTON0_NODE DT_ALIAS(button0)
#elif DT_NODE_EXISTS(DT_NODELABEL(sw0))
#define BUTTON0_NODE DT_NODELABEL(sw0)
#elif DT_NODE_EXISTS(DT_NODELABEL(button0))
#define BUTTON0_NODE DT_NODELABEL(button0)
#else
#define BUTTON0_NODE DT_INVALID_NODE
#endif

#if DT_NODE_EXISTS(BUTTON0_NODE)
#define BUTTON0_DEV DT_PHANDLE(BUTTON0_NODE, gpios)
#define BUTTON0_PIN DT_PHA(BUTTON0_NODE, gpios, pin)
#define BUTTON0_FLAGS DT_PHA(BUTTON0_NODE, gpios, flags)

static const struct device *const button_dev = DEVICE_DT_GET(BUTTON0_DEV);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(BUTTON0_NODE, gpios,
							      {0});
static struct gpio_callback button_cb_data;

static struct k_work *button_work;

static void button_cb(const struct device *port, struct gpio_callback *cb,
		      gpio_port_pins_t pins)
{
	k_work_submit(button_work);
}
#endif

static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
						     {0});


int ledInit() // most can be removed after testing
{
    k_msleep(1000);
    if (!gpio_is_ready_dt(&led))
    {
        printk("Error: LED device %s is not ready\n", led.port->name);
        return -1;
    }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT);
    gpio_pin_set_dt(&led, 1);
    k_msleep(100);
    gpio_pin_set_dt(&led, 0);
	return 0;
}

int ledSet(bool value)
{
    gpio_pin_set_dt(&led, value);
    return 0;	
}

void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
    // event to happen when button is pressed
	btnPressed();
}

int buttonInit(struct k_work *button_pressed)
{
#if DT_NODE_EXISTS(BUTTON0_NODE)
	int err;

	err = gpio_pin_configure(button_dev, BUTTON0_PIN,
				 BUTTON0_FLAGS | GPIO_INPUT);
	if (err) {
		return err;
	}

	static struct gpio_callback gpio_cb;

	err = gpio_pin_interrupt_configure(button_dev, BUTTON0_PIN,
					   GPIO_INT_EDGE_TO_ACTIVE);
	if (err) {
		return err;
	}

	button_work = button_pressed;

	gpio_init_callback(&gpio_cb, button_cb, BIT(BUTTON0_PIN));
	gpio_add_callback(button_dev, &gpio_cb);
#else
	printk("WARNING: Buttons not supported on this board.\n");
#endif

	return 0;
}

void init()
{
    bluetoothInit();
}


int board_init(struct k_work *button_pressed)
{
	int err;

	err = ledInit();
	if (err) {
		return err;
	}

	return buttonInit(button_pressed);
}

void boardOutputNumber(bt_mesh_output_action_t action, uint32_t number)
{

}

void boardProvComplete(void)
{
}