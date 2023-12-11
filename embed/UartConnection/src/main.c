/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <string.h>

#include "nodeData.h"
#include "groupData.h"
#include "uart.h"
#include "compc.h"

#define SLEEP_TIME_MS 10

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
															  {0});
static struct gpio_callback button_cb_data;

/*
 * The led0 devicetree alias is optional. If present, we'll use it
 * to turn on the LED whenever the button is pressed.
 */
static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
													 {0});

void button_pressed(const struct device *dev, struct gpio_callback *cb,
					uint32_t pins)
{
	// printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

// Important structs for storing information
struct Node node;
struct Group group[MAX_GROUPS_ALLOWED];

int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&button))
	{
		printk("Error: button device %s is not ready\n",
			   button.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0)
	{
		printk("Error %d: failed to configure %s pin %d\n",
			   ret, button.port->name, button.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&button,
										  GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0)
	{
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			   ret, button.port->name, button.pin);
		return 0;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	if (led.port && !gpio_is_ready_dt(&led))
	{
		printk("Error %d: LED device %s is not ready; ignoring it\n",
			   ret, led.port->name);
		led.port = NULL;
	}
	if (led.port)
	{
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (ret != 0)
		{
			printk("Error %d: failed to configure LED device %s pin %d\n",
				   ret, led.port->name, led.pin);
			led.port = NULL;
		}
		else
		{
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}

	uartSetup();
	int buttonPressed = 0;
	int ledState = 0;
	int ledActivated = 0;

	// Reset if groups are filled
	for (int i = 0; i < MAX_GROUPS_ALLOWED; i++)
	{
		group[i].groupFilled = '0';

		// Reset name so we can check if a group already exists
		for (int n = 0; n < MAX_NAME_LENGTH; n++)
		{
			group[i].groupName[n] = '0';
		}

		// Reset sensor so we can check if a sensor has already been added
		for (int s = 0; s < MAX_SENSORS_IN_GROUP; s++)
		{
			group[i].sensors[s].sensorFilled = '0';
		}

		// Reset actuator so we can check if a actuator has already been added
		for (int s = 0; s < MAX_ACTUATORS_IN_GROUP; s++)
		{
			group[i].actuators[s].actuatorFilled = '0';
		}
	}
	strcpy(nodes[0], "AddNode STM32_______ AddSensor Button______ STM_Button__ AddActuator LED_________ STM_LED_____ false\n");
	strcpy(groups[0], "AddGroup Test_Group__ AddSensor Application_ Button______ App_Button__ AddSensor STM32_______ Button_______ STM_Button__ AddActuator STM32_______ LED_________ STM_LED_____\n");
	//strcpy(groups[0], "AddGroup Test_Group__ AddSensor Application_ Button______ App_Button__ AddActuator STM32_______ LED_________ STM_LED_____\n");
	addNode(&node, nodes[0]);
	addGroup(group, groups[0]);

	printk("Start\n");
	if (led.port)
	{
		while (1)
		{
			/* If we have an LED, match its state to the button's. */
			int val = gpio_pin_get_dt(&button);

			if (val >= 1 && buttonPressed == 0)
			{
				printk("TriggerSensor STM32_______ STM_Button__\n");
				buttonPressed = 1;
			}
			else if (val == 0 && buttonPressed == 1)
			{
				buttonPressed = 0;
			}

			if (ledState == 1)
			{
				gpio_pin_set_dt(&led, 1);
				if (ledActivated == 0)
				{
					printk("UpdateAppActuator STM32_______ STM_LED_____ true\n");
					ledActivated = 1;
				}
			}
			else
			{
				gpio_pin_set_dt(&led, 0);
				if (ledActivated == 1)
				{
					printk("UpdateAppActuator STM32_______ STM_LED_____ false\n");
					ledActivated = 0;
				}
			}

			readPc(&node, group, &ledState); // Reads uart output from the pc

			k_msleep(SLEEP_TIME_MS);
		}
	}
	return 0;
}
