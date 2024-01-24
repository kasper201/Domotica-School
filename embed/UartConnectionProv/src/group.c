#include "group.h"

void subscribeToGroup(uint16_t groupAddress, uint16_t elementAddress, uint16_t mod_id, uint16_t netKeyIndex, uint16_t address)
{
    int err = 0;
    netKeyIndex = 0x0000;
    // Using the Bluetooth Mesh Configuration Client API to add a subscription
    err = bt_mesh_cfg_cli_mod_sub_add(netKeyIndex, address, elementAddress, groupAddress, mod_id, NULL);
    if (err) {
        printk("Subscription failed (err %d)\n", err);
    }
    printk("Sub Network Index: 0x%04x, Address: 0x%04x\n", netKeyIndex, address);
    printk("Subscription add (err: %d)\n", err);
}

void unsubscribeFromGroup(uint16_t groupAddress, uint16_t elementAddress, uint16_t mod_id, uint16_t netKeyIndex, uint16_t address)
{
    int err = 0;
    // Using the Bluetooth Mesh Configuration Client API to add a subscription
    err = bt_mesh_cfg_cli_mod_sub_del(netKeyIndex, address, elementAddress, groupAddress, mod_id, NULL);
    if (err) {
        printk("Unsubscription failed (err %d)\n", err);
    }
    printk("unsub Network Index: 0x%04x, Address: 0x%04x\n", netKeyIndex, address);
    printk("Subscription remove (err: %d)\n", err);
}