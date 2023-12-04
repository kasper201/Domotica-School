#ifndef NODEDATA_H_
#define NODEDATA_H_

#include "main.h"

extern char nodes[MAX_NODES_ALLOWED][MAX_INFO_STRING_LENGTH];

void nodesToPc(struct Node* node);
void addNode(struct Node *node, char* nodeString);

#endif //NODEDATA_H_