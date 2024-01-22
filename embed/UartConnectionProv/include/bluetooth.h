#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint.h>
#include <stdbool.h>

bool onoffVal();
void getNetIdx(uint16_t *netIdx);
uint16_t getAddr();
uint16_t getElem(uint8_t index);
int gen_onoff_send(bool val, uint16_t groupAddress); //send onoff message to group
int bluetoothInit();
void provMain();
void btnPressed();

#endif // BLUETOOTH_H_