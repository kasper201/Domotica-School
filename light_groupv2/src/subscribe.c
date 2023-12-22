#include "subscribe.h"

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