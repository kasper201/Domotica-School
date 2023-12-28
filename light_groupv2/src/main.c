/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *  @brief Nordic mesh light sample
 */
#include <zephyr/bluetooth/bluetooth.h>
#include <bluetooth/mesh/models.h>
#include <bluetooth/mesh/dk_prov.h>
#include <dk_buttons_and_leds.h>
#include "model_handler.h"
#include "smp_bt.h"
#include <zephyr/bluetooth/mesh/access.h>
#include <stdint.h>
#include <stdbool.h>
#include <zephyr/bluetooth/mesh/cfg_cli.h>
#include <zephyr/drivers/gpio.h>
#define SLEEP_TIME_MS	1
//test
/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE	DT_ALIAS(sw0)
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

void boardOutputNumber(bt_mesh_output_action_t action, uint32_t number)
{

}

void boardProvComplete(void)
{
}
/* Provisioning */
static int output_number(bt_mesh_output_action_t action, uint32_t number)
{
	printk("OOB Number: %u\n", number);

	boardOutputNumber(action, number);

	return 0;
}

void clear_provisioning_data(void)
{
    int err;

    // Clear provisioning data from settings
    err = settings_delete("bt/mesh");
    if (err) {
        printk("Failed to clear provisioning data (err %d)\n", err);
        return;
    }

    printk("Provisioning data cleared\n");
}
static void prov_complete(uint16_t net_idx, uint16_t addr)
{
	extern uint16_t extern_net_idx;
	extern uint16_t extern_addr;
	extern_net_idx = net_idx;
	extern_addr = addr;
	boardProvComplete();
	printk("Provisioning completed. Network Index: 0x%04x, Address: 0x%04x\n",
           net_idx, addr);

}

static void prov_reset(void)
{
	bt_mesh_prov_disable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT | BT_MESH_PROV_REMOTE);//disable mesh provisioning
	clear_provisioning_data();//reset provisioning data
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
	//bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
}
static uint8_t dev_uuid[16];
static const struct bt_mesh_prov prov = {
	.uuid = dev_uuid,
	.output_size = 4,
	.output_actions = BT_MESH_DISPLAY_NUMBER,
	.output_number = output_number,
	.complete = prov_complete,
	.reset = prov_reset,
};

static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	dk_leds_init();
	dk_buttons_init(NULL);
    printk("Led en Buttons initialized\n");

	err = bt_mesh_init(&prov, model_handler_init());
	if (err) {
		printk("Initializing mesh failed (err %d)\n", err);
		return;
	}
    printk("Mesh provision initialized\n");

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
		printk("settings : %d\n",settings_load());
	}

	/* This will be a no-op if settings_load() loaded provisioning info */
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);

	printk("Mesh initialized\n");

	if (IS_ENABLED(CONFIG_SOC_SERIES_NRF52X) && IS_ENABLED(CONFIG_MCUMGR_TRANSPORT_BT)) {
		err = smp_dfu_init();
		if (err) {
			printk("Unable to initialize DFU (err %d)\n", err);
		}
	}
	printk("end bt_ready\n");
}

void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	uint16_t button_net_idx = 0;
	uint16_t button_addr = 0;
	getNetIdx(&button_net_idx);
	getAddr(&button_addr);
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
	printk("Button pressed net_idx: 0x%04x and addr: 0x%04x\n", button_net_idx, button_addr);
	uint16_t elem_addr = button_addr;
	uint16_t sub_addr = 0xC000;
	uint16_t mod_id = 0x1000;
	int err;
	uint8_t status = 0;
	if(elem_addr = 0)
	{
		printk("no addres found");
	}
	else{
		/*err = bt_mesh_cfg_cli_mod_sub_add(button_net_idx, button_addr, elem_addr, sub_addr, mod_id,
												  &status);
		if (err) {
		printk("sub failed (err %d)\n", err);
		}*/
	}
	return;
}
void subscribe(uint16_t net_idx, uint16_t addr,uint16_t elem_addr,uint16_t sub_addr,uint16_t mod_id)
{
	int err;
	uint8_t status = 0;

	err = bt_mesh_cfg_cli_mod_sub_add(net_idx, addr, elem_addr, sub_addr, mod_id,
												  &status);
	if (err) {
		printk("sub failed (err %d)\n", err);
	}
	printk("Sub Network Index: 0x%04x, Address: 0x%04x\n",
           net_idx, addr);
	printk("Sub add (err: %d, status: %d)\n", err,
				   status);
				   
}
static void button_init(void)
{
	int ret;

	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n",
		       button.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button.port->name, button.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&button,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button.port->name, button.pin);
		return 0;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	if (led.port && !gpio_is_ready_dt(&led)) {
		printk("Error %d: LED device %s is not ready; ignoring it\n",
		       ret, led.port->name);
		led.port = NULL;
	}
	if (led.port) {
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led.port->name, led.pin);
			led.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}
}
uint16_t extern_net_idx = 0;
uint16_t extern_addr = 0;
void getNetIdx(uint16_t *input)
{
	extern uint16_t extern_net_idx;
	*input = extern_net_idx;
}
void getAddr(uint16_t *input)
{
	extern uint16_t extern_addr;
	*input = extern_addr;
}
int main(void)
{
	int err;
	extern uint16_t extern_net_idx;
	extern uint16_t extern_addr;
	printk("Initializing...\n");

	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}
	
	printk("bt_ready ok\n");
	button_init();
	printk("Press the button\n");
	return 0;
	printk("End Main\n");
}
