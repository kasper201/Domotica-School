#ifndef GROUP_H_
#define GROUP_H_

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/mesh.h>
#include <zephyr/bluetooth/mesh/access.h>
#include <stdint.h>
#include <stdbool.h>
#include <zephyr/bluetooth/mesh/cfg_cli.h>
#include <zephyr/drivers/gpio.h>

void subscribeToGroup(uint16_t groupAddress, uint16_t elementAddress, uint16_t mod_id, uint16_t netKeyIndex, uint16_t address);

#endif // GROUP_H_