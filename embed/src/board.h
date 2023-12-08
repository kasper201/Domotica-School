#ifndef BOARD_H_
#define BOARD_H_

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/mesh.h>

void init();
int ledSet(bool value);
void boardOutputNumber(bt_mesh_output_action_t action, uint32_t number);
void boardProvComplete(void);

#endif // BOARD_H_
