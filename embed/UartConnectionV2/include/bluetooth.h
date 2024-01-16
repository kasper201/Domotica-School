#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint.h>
#include <stdbool.h>

bool onoffVal();
void getNetIdx(uint16_t *netIdx);
int gen_onoff_send(bool val, uint16_t groupAddress); //send onoff message to group
int bluetoothInit();
void btnPressed();

#endif // BLUETOOTH_H_