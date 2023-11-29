#include <zephyr.h>
#include <device.h>

void main(void)
{
    struct device *led_dev;

    led_dev = device_get_binding("GPIO_0");
    if (led_dev == NULL) {
        printk("Error: didn't find GPIO_0 device\n");
        return;
    }

    while (1) {
        gpio_pin_set(led_dev, 13, 1);
        k_msleep(1000);
        gpio_pin_set(led_dev, 13, 0);
        k_msleep(1000);
    }
}
