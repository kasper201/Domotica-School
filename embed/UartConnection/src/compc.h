#ifndef COMPC_H_
#define COMPC_H_

#include "nodeData.h"
#include <stdbool.h>

void readPc(char (*nodes)[MAX_NODE_INFO_STRING_LENGTH], bool *ledState); //bool ledState is temporary
void extractStrings(const char *input, char *first, char *second);          //Gives the first wordt of a string and what remains

#endif //COMPC_H_