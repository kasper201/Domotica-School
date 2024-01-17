#include "triggered.h"

#include <string.h>

void triggeredGroup(struct Group group, struct Node node, int *ledState)
{
    for (int a = 0; a < MAX_ACTUATORS_IN_GROUP; a++)
    {
        if (strstr(group.actuators[a].nodeName, node.nodeName))
        {
            if (strstr(group.actuators[a].actuatorName, LED_NAME))
            {
                if (*ledState == 1)
                {
                    *ledState = 0;
                }
                else
                {
                    *ledState = 1;
                }
            }
        }
    }
}