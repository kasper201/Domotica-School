#include <zephyr/bluetooth/mesh.h>
#include <zephyr/drivers/gpio.h>

#include "board.h"


#if DT_NODE_EXISTS(DT_ALIAS(led0))
static const struct device *const led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0));


// gathered define and inits from zephyr sample/bluetooth/mesh They might have been altered and are not like the original anymore.
/* Locate button0 as alias or label by sw0 or button0 */
#if DT_NODE_EXISTS(DT_ALIAS(sw0))
#define BUTTON0 DT_ALIAS(sw0)
#elif DT_NODE_EXISTS(DT_ALIAS(button0))
#define BUTTON0 DT_ALIAS(button0)
#elif DT_NODE_EXISTS(DT_NODELABEL(sw0))
#define BUTTON0 DT_NODELABEL(sw0)
#elif DT_NODE_EXISTS(DT_NODELABEL(button0))
#define BUTTON0 DT_NODELABEL(button0)
#else
#define BUTTON0 DT_INVALID_NODE
#endif

#if DT_NODE_EXISTS(LED0)
#define LED0_DEV DT_PHANDLE(LED0, gpios)
#define LED0_PIN DT_PHA(LED0, gpios, pin)
#define LED0_FLAGS DT_PHA(LED0, gpios, flags)

static const struct device *const led_dev = DEVICE_DT_GET(LED0_DEV);
#endif /* LED0 */

#if DT_NODE_EXISTS(BUTTON0)
#define BUTTON0_DEV DT_PHANDLE(BUTTON0, gpios)
#define BUTTON0_PIN DT_PHA(BUTTON0, gpios, pin)
#define BUTTON0_FLAGS DT_PHA(BUTTON0, gpios, flags)

static const struct device *const button_dev = DEVICE_DT_GET(BUTTON0_DEV);
static struct k_work *button_work;

static void button_cb(const struct device *port, struct gpio_callback *cb,
		      gpio_port_pins_t pins)
{
	k_work_submit(button_work);
}
#endif /* BUTTON0 */

/* not my code!!!!! Originally from zephyr sample
static int button_init(struct k_work *button_pressed)
{
#if DT_NODE_EXISTS(BUTTON0)
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
}*/

void ledInit() // most can be removed after testing
{
    gpio_pin_configure_dt(&led0, GPIO_OUTPUT);
    gpio_pin_set_dt(led0, 1);
    k_msleep(500);
    gpio_pin_set_dt(led0, 0);
}

void ledSet(bool value)
{
    gpio_pin_set_dt(led0, value);
}