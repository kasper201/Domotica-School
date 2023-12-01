#ifndef COMPC_H_
#define COMPC_H_

#include "nodeData.h"
#include <stdbool.h>

void readPc(char (*nodes)[MAX_NODE_INFO_STRING_LENGTH], bool *ledState); //bool ledState is temporary

#endif //COMPC_H_