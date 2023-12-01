#include "groupData.h"

#include <zephyr/sys/printk.h>

//Defines group strings
char groups[MAX_GROUPS_ALLOWED][MAX_INFO_STRING_LENGTH];

//Sends groups to application
void groupsToPc(char (*groups)[MAX_INFO_STRING_LENGTH])
{
    printk("%s", groups[0]);
}