#include "group.h"

#include <inttypes.h>
uint16_t extern_net_idx = 0;
uint16_t extern_addr = 0;

/**
 * @brief Get the node Net_Idx
 * 
 * @param input pointer input
 */
void getNetIdx(uint16_t *input)
{
	extern uint16_t extern_net_idx;
	*input = extern_net_idx;
}

/**
 * @brief Get the node Addr 
 * 
 * @param input pointer input
 */
void getAddr(uint16_t *input)
{
	extern uint16_t extern_addr;
	*input = extern_addr;
}

/**
 * @brief subscribe node to group
 * 
 * @param net_idx	node net_idx
 * @param addr		node addr
 * @param elem_addr node elem_addr
 * @param sub_addr  group adres
 * @param mod_id  	node model ID
 */
void subscribe(uint16_t net_idx, uint16_t addr,uint16_t elem_addr,uint16_t sub_addr,uint16_t mod_id)
{
	int err;

	err = bt_mesh_cfg_cli_mod_sub_add(net_idx, addr, elem_addr, sub_addr, mod_id,NULL);
	if (err) {
		printk("sub failed (err %d)\n", err);
	}
	printk("Sub Network Index: 0x%04x, Address: 0x%04x\n",
           net_idx, addr);
	printk("Sub add (err: %d)\n", err);
				   
}