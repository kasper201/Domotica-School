#include "extern_var.h"

uint16_t extern_net_idx = 0;
uint16_t extern_addr = 0;

void SetNetidx(uint16_t input)
{
    extern_net_idx = input;
}

void SetAddr(uint16_t input)
{
    extern_addr = input;
}
