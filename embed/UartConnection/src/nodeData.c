#include "nodeData.h"

#include <zephyr/sys/printk.h>

//Defines char nodes
char nodes[MAX_NODES_ALLOWED][MAX_NODE_INFO_STRING_LENGTH];

//Sends all nodes currently connected to the pc
void nodesToPc(char (*nodes)[MAX_NODE_INFO_STRING_LENGTH])
{
    printk("%s", nodes[0]);
}