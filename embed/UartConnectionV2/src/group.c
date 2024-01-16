#include "group.h"

void subscribeToGroup(uint16_t groupAddress, uint16_t elementAddress, uint16_t mod_id, uint16_t netKeyIndex, uint16_t address)
{
    int err = 0;
    uint8_t status = 1;
    // Using the Bluetooth Mesh Configuration Client API to add a subscription
    err = bt_mesh_cfg_cli_mod_sub_add(netKeyIndex, address, elementAddress, groupAddress, mod_id, &status);
    //err = bt_mesh_cfg_cli_mod_sub_add(0x0000, 0x0002, 0x0002, 0xC000, 0x1000, &status);
    if (err) {
        printk("Subscription failed (err %d)\n", err);
    }
    printk("Sub Network Index: 0x%04x, Address: 0x%04x\n", netKeyIndex, address);
    printk("Subscription add (err: %d, status: %d)\n", err, status);
}

void unsubscribeFromGroup(uint16_t groupAddress, uint16_t elementAddress, uint16_t mod_id, uint16_t netKeyIndex, uint16_t address)
{
    int err = 0;
    uint8_t status = 0;
    // Using the Bluetooth Mesh Configuration Client API to add a subscription
    err = bt_mesh_cfg_cli_mod_sub_del(netKeyIndex, address, elementAddress, groupAddress, mod_id, &status);
    if (err) {
        printk("Unsubscription failed (err %d)\n", err);
    }
    printk("unsub Network Index: 0x%04x, Address: 0x%04x\n", netKeyIndex, address);
    printk("Subscription remove (err: %d, status: %d)\n", err, status);
}