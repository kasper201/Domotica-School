#ifndef GROUPDATA_H_
#define GROUPDATA_H_

#include "nodeData.h"
#include "main.h"

extern char groups[MAX_GROUPS_ALLOWED][MAX_INFO_STRING_LENGTH];

void groupsToPc(char (*groups)[MAX_INFO_STRING_LENGTH]);

#endif //GROUPDATA_H_