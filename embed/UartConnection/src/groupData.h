#ifndef GROUPDATA_H_
#define GROUPDATA_H_

#include "nodeData.h"
#include "main.h"

extern char groups[MAX_GROUPS_ALLOWED][MAX_INFO_STRING_LENGTH];

void groupsToPc(struct Group *group);
void addGroup(struct Group *group, char* groupString);
void createGroup(struct Group* group, char* groupString);
void updateGroup(struct Group* group, char* groupString);

#endif //GROUPDATA_H_