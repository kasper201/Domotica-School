#ifndef GROUPDATA_H_
#define GROUPDATA_H_

#include "nodeData.h"

#define MAX_GROUPS_ALLOWED 10       //Max number of groups allowed to be active at the same time

extern char groups[MAX_GROUPS_ALLOWED][MAX_INFO_STRING_LENGTH];

void groupsToPc(char (*groups)[MAX_INFO_STRING_LENGTH]);

#endif //GROUPDATA_H_