/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>
#include <stdlib.h>
#include <zephyr/kernel.h>

#include <zephyr/shell/shell.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/mesh.h>
#include <zephyr/bluetooth/mesh/shell.h>

// LED control
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/hwinfo.h>

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

int ledSet(bool value)
{
    gpio_pin_set_dt(&led, value);
    return 0;	
}

#define OP_ONOFF_GET       BT_MESH_MODEL_OP_2(0x82, 0x01)
#define OP_ONOFF_SET       BT_MESH_MODEL_OP_2(0x82, 0x02)
#define OP_ONOFF_SET_UNACK BT_MESH_MODEL_OP_2(0x82, 0x03)
#define OP_ONOFF_STATUS    BT_MESH_MODEL_OP_2(0x82, 0x04)

static struct bt_mesh_cfg_cli cfg_cli;

#if defined(CONFIG_BT_MESH_DFD_SRV)
static struct bt_mesh_dfd_srv dfd_srv;
#endif

#if defined(CONFIG_BT_MESH_SAR_CFG_CLI)
static struct bt_mesh_sar_cfg_cli sar_cfg_cli;
#endif

#if defined(CONFIG_BT_MESH_PRIV_BEACON_CLI)
static struct bt_mesh_priv_beacon_cli priv_beacon_cli;
#endif

#if defined(CONFIG_BT_MESH_SOL_PDU_RPL_CLI)
static struct bt_mesh_sol_pdu_rpl_cli srpl_cli;
#endif


#if defined(CONFIG_BT_MESH_OD_PRIV_PROXY_CLI)
static struct bt_mesh_od_priv_proxy_cli od_priv_proxy_cli;
#endif

#if defined(CONFIG_BT_MESH_LARGE_COMP_DATA_CLI)
struct bt_mesh_large_comp_data_cli large_comp_data_cli;
#endif

BT_MESH_SHELL_HEALTH_PUB_DEFINE(health_pub);

static const char *const onoff_str[] = { "off", "on" };

static struct {
	bool val;
	uint8_t tid;
	uint16_t src;
	uint32_t transition_time;
	struct k_work_delayable work;
} onoff;

/* OnOff messages' transition time and remaining time fields are encoded as an
 * 8 bit value with a 6 bit step field and a 2 bit resolution field.
 * The resolution field maps to:
 * 0: 100 ms
 * 1: 1 s
 * 2: 10 s
 * 3: 20 min
 */
static const uint32_t time_res[] = {
	100,
	MSEC_PER_SEC,
	10 * MSEC_PER_SEC,
	10 * 60 * MSEC_PER_SEC,
};

static inline int32_t model_time_decode(uint8_t val)
{
	uint8_t resolution = (val >> 6) & BIT_MASK(2);
	uint8_t steps = val & BIT_MASK(6);

	if (steps == 0x3f) {
		return SYS_FOREVER_MS;
	}

	return steps * time_res[resolution];
}

static inline uint8_t model_time_encode(int32_t ms)
{
	if (ms == SYS_FOREVER_MS) {
		return 0x3f;
	}

	for (int i = 0; i < ARRAY_SIZE(time_res); i++) {
		if (ms >= BIT_MASK(6) * time_res[i]) {
			continue;
		}

		uint8_t steps = DIV_ROUND_UP(ms, time_res[i]);

		return steps | (i << 6);
	}

	return 0x3f;
}

static int onoff_status_send(struct bt_mesh_model *model, //? what is dis for
			     struct bt_mesh_msg_ctx *ctx)
{
	uint32_t remaining;

	BT_MESH_MODEL_BUF_DEFINE(buf, OP_ONOFF_STATUS, 3);
	bt_mesh_model_msg_init(&buf, OP_ONOFF_STATUS);

	remaining = k_ticks_to_ms_floor32(
			    k_work_delayable_remaining_get(&onoff.work)) +
		    onoff.transition_time;

	/* Check using remaining time instead of "work pending" to make the
	 * onoff status send the right value on instant transitions. As the
	 * work item is executed in a lower priority than the mesh message
	 * handler, the work will be pending even on instant transitions.
	 */
	if (remaining) {
		net_buf_simple_add_u8(&buf, !onoff.val);
		net_buf_simple_add_u8(&buf, onoff.val);
		net_buf_simple_add_u8(&buf, model_time_encode(remaining));
	} else {
		net_buf_simple_add_u8(&buf, onoff.val);
	}

	return bt_mesh_model_send(model, ctx, &buf, NULL, NULL);
}

static void onoff_timeout(struct k_work *work)
{
	if (onoff.transition_time) {
		/* Start transition.
		 *
		 * The LED should be on as long as the transition is in
		 * progress, regardless of the target value, according to the
		 * Bluetooth Mesh Model specification, section 3.1.1.
		 */
		ledSet(true);

		k_work_reschedule(&onoff.work, K_MSEC(onoff.transition_time));
		onoff.transition_time = 0;
		return;
	}

	ledSet(onoff.val);
}

static int gen_onoff_get(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct net_buf_simple *buf)
{
	onoff_status_send(model, ctx);
	return 0;
}

static int gen_onoff_set_unack(struct bt_mesh_model *model,
			       struct bt_mesh_msg_ctx *ctx,
			       struct net_buf_simple *buf)
{
	uint8_t val = net_buf_simple_pull_u8(buf);
	uint8_t tid = net_buf_simple_pull_u8(buf);
	int32_t trans = 0;
	int32_t delay = 0;

	if (buf->len) {
		trans = model_time_decode(net_buf_simple_pull_u8(buf));
		delay = net_buf_simple_pull_u8(buf) * 5;
	}

	/* Only perform change if the message wasn't a duplicate and the
	 * value is different.
	 */
	if (tid == onoff.tid && ctx->addr == onoff.src) {
		/* Duplicate */
		return 0;
	}

	if (val == onoff.val) {
		/* No change */
		return 0;
	}

	printk("set: %s delay: %d ms time: %d ms\n", onoff_str[val], delay,
	       trans);

	onoff.tid = tid;
	onoff.src = ctx->addr;
	onoff.val = val;
	onoff.transition_time = trans;

	/* Schedule the next action to happen on the delay, and keep
	 * transition time stored, so it can be applied in the timeout.
	 */
	k_work_reschedule(&onoff.work, K_MSEC(delay));

	return 0;
}

static int gen_onoff_set(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct net_buf_simple *buf)
{
	(void)gen_onoff_set_unack(model, ctx, buf);
	onoff_status_send(model, ctx);

	return 0;
}

static const struct bt_mesh_model_op gen_onoff_srv_op[] = {
	{ OP_ONOFF_GET,       BT_MESH_LEN_EXACT(0), gen_onoff_get },
	{ OP_ONOFF_SET,       BT_MESH_LEN_MIN(2),   gen_onoff_set },
	{ OP_ONOFF_SET_UNACK, BT_MESH_LEN_MIN(2),   gen_onoff_set_unack },
	BT_MESH_MODEL_OP_END,
};

/* Generic OnOff Client */

static int gen_onoff_status(struct bt_mesh_model *model,
			    struct bt_mesh_msg_ctx *ctx,
			    struct net_buf_simple *buf)
{
	uint8_t present = net_buf_simple_pull_u8(buf);

	if (buf->len) {
		uint8_t target = net_buf_simple_pull_u8(buf);
		int32_t remaining_time =
			model_time_decode(net_buf_simple_pull_u8(buf));

		printk("OnOff status: %s -> %s: (%d ms)\n", onoff_str[present],
		       onoff_str[target], remaining_time);
		return 0;
	}

	printk("OnOff status: %s\n", onoff_str[present]);

	return 0;
}

static const struct bt_mesh_model_op gen_onoff_cli_op[] = {
	{OP_ONOFF_STATUS, BT_MESH_LEN_MIN(1), gen_onoff_status},
	BT_MESH_MODEL_OP_END,
};

static struct bt_mesh_model root_models[] = 
{
	BT_MESH_MODEL_CFG_SRV,
	BT_MESH_MODEL_CFG_CLI(&cfg_cli),
	BT_MESH_MODEL_HEALTH_SRV(&bt_mesh_shell_health_srv, &health_pub),
	BT_MESH_MODEL_HEALTH_CLI(&bt_mesh_shell_health_cli),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op, NULL,
		      NULL),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_CLI, gen_onoff_cli_op, NULL,
		      NULL),
#if defined(CONFIG_BT_MESH_DFD_SRV)
	BT_MESH_MODEL_DFD_SRV(&dfd_srv),
#else
#if defined(CONFIG_BT_MESH_SHELL_DFU_SRV)
	BT_MESH_MODEL_DFU_SRV(&bt_mesh_shell_dfu_srv),
#elif defined(CONFIG_BT_MESH_SHELL_BLOB_SRV)
	BT_MESH_MODEL_BLOB_SRV(&bt_mesh_shell_blob_srv),
#endif
#if defined(CONFIG_BT_MESH_SHELL_DFU_CLI)
	BT_MESH_MODEL_DFU_CLI(&bt_mesh_shell_dfu_cli),
#elif defined(CONFIG_BT_MESH_SHELL_BLOB_CLI)
	BT_MESH_MODEL_BLOB_CLI(&bt_mesh_shell_blob_cli),
#endif
#endif /* CONFIG_BT_MESH_DFD_SRV */
#if defined(CONFIG_BT_MESH_SHELL_RPR_CLI)
	BT_MESH_MODEL_RPR_CLI(&bt_mesh_shell_rpr_cli),
#endif
#if defined(CONFIG_BT_MESH_RPR_SRV)
	BT_MESH_MODEL_RPR_SRV,
#endif

#if defined(CONFIG_BT_MESH_SAR_CFG_SRV)
	BT_MESH_MODEL_SAR_CFG_SRV,
#endif
#if defined(CONFIG_BT_MESH_SAR_CFG_CLI)
	BT_MESH_MODEL_SAR_CFG_CLI(&sar_cfg_cli),
#endif

#if defined(CONFIG_BT_MESH_OP_AGG_SRV)
	BT_MESH_MODEL_OP_AGG_SRV,
#endif
#if defined(CONFIG_BT_MESH_OP_AGG_CLI)
	BT_MESH_MODEL_OP_AGG_CLI,
#endif

#if defined(CONFIG_BT_MESH_LARGE_COMP_DATA_SRV)
	BT_MESH_MODEL_LARGE_COMP_DATA_SRV,
#endif
#if defined(CONFIG_BT_MESH_LARGE_COMP_DATA_CLI)
	BT_MESH_MODEL_LARGE_COMP_DATA_CLI(&large_comp_data_cli),
#endif

#if defined(CONFIG_BT_MESH_PRIV_BEACON_SRV)
	BT_MESH_MODEL_PRIV_BEACON_SRV,
#endif
#if defined(CONFIG_BT_MESH_PRIV_BEACON_CLI)
	BT_MESH_MODEL_PRIV_BEACON_CLI(&priv_beacon_cli),
#endif
#if defined(CONFIG_BT_MESH_OD_PRIV_PROXY_CLI)
	BT_MESH_MODEL_OD_PRIV_PROXY_CLI(&od_priv_proxy_cli),
#endif
#if defined(CONFIG_BT_MESH_SOL_PDU_RPL_CLI)
	BT_MESH_MODEL_SOL_PDU_RPL_CLI(&srpl_cli),
#endif
#if defined(CONFIG_BT_MESH_OD_PRIV_PROXY_SRV)
	BT_MESH_MODEL_OD_PRIV_PROXY_SRV,
#endif
};

static struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(0, root_models, BT_MESH_MODEL_NONE),
};

static const struct bt_mesh_comp comp = {
	.cid = CONFIG_BT_COMPANY_ID,
	.elem = elements,
	.elem_count = ARRAY_SIZE(elements),
};

static void bt_ready(int err)
{
	if (err && err != -EALREADY) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	err = bt_mesh_init(&bt_mesh_shell_prov, &comp);
	if (err) {
		printk("Initializing mesh failed (err %d)\n", err);
		return;
	}

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		printk("LoadingConfig\n");
		settings_load();
		printk("Config Loaded\n");
	}

	printk("Mesh initialized\n");

	if (bt_mesh_is_provisioned()) {
		printk("Mesh network restored from flash\n");
	} else {
		printk("Use \"prov pb-adv on\" or \"prov pb-gatt on\" to "
			    "enable advertising\n");
	}
}

int main(void)
{
	int err;

	printk("Initializing...\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err && err != -EALREADY) {
		printk("Bluetooth init failed (err %d)\n", err);
	}

	k_work_init_delayable(&onoff.work, onoff_timeout);

	printk("Press the <Tab> button for supported commands.\n");
	printk("Before any Mesh commands you must run \"mesh init\"\n");
	return 0;
}
