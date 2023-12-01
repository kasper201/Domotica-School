#ifndef COMPC_H_
#define COMPC_H_

#include "nodeData.h"
#include "groupData.h"

void readPc(char (*nodes)[MAX_INFO_STRING_LENGTH], char (*groups)[MAX_INFO_STRING_LENGTH], int *ledState); //bool ledState is temporary

#endif //COMPC_H_