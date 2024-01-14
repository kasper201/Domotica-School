#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint.h>
#include <stdbool.h>

int gen_onoff_send(bool val, uint16_t groupAddress); //send onoff message to group
int bluetoothInit();
void btnPressed();

#endif // BLUETOOTH_H_