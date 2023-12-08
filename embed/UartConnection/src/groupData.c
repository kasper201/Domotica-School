#include "groupData.h"

#include <zephyr/sys/printk.h>
#include <string.h>

//Defines group strings
char groups[MAX_GROUPS_ALLOWED][MAX_INFO_STRING_LENGTH];

//Sends groups to application
void groupsToPc(struct Group *group)
{
    //printk("%s", groups[0]);
    for(int i = 0; i < MAX_GROUPS_ALLOWED; i++)
    {
        if(group[i].groupFilled == '1')
        {
            printk("AddGroup %s ", group[i].groupName);
            printk("AddSensor %s %s %s ", group[i].sensors->nodeName, group[i].sensors->sensorType, group[i].sensors->sensorName);
            printk("AddActuator %s %s %s\n", group[i].actuators->nodeName, group[i].actuators->actuatorType, group[i].actuators->actuatorName);
        }
    }
}

//Add a node
void addGroup(struct Group *group, char* groupString)
{
    int free = 0;
    while (group[free].groupFilled == '1')
    {
        free++;
        if(free >= MAX_GROUPS_ALLOWED)
        {
            printk("No free group left");
            break;
        }
    }

    if(group[free].groupFilled == '0')
    {
        if(strstr(groupString, "AddGroup"))
        {
            char* keyword = "AddGroup ";
            char* position = strstr(groupString, keyword);

            // Move the pointer to the start of nodeName (8 characters after "AddGroup")
            position += strlen(keyword);

            // Copy the nodeName into the node structure
            strncpy(group[free].groupName, position, MAX_NAME_LENGTH - 1);
            group[free].groupName[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

            keyword = "AddSensor ";
            if(strstr(groupString, keyword))
            {
                position = strstr(groupString, keyword);
                position += strlen(keyword);
                
                // Copy the sensorType into the node structure
                strncpy(group[free].sensors->nodeName, position, MAX_NAME_LENGTH - 1);
                group[free].sensors->nodeName[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

                // Copy the sensorType into the group structure
                position += MAX_NAME_LENGTH;
                strncpy(group[free].sensors->sensorType, position, MAX_NAME_LENGTH - 1);
                group[free].sensors->sensorType[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

                // Copy the sensorName into the node structure
                position += MAX_NAME_LENGTH;
                strncpy(group[free].sensors->sensorName, position, MAX_NAME_LENGTH -1);
                group[free].sensors->sensorName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
            }

            keyword = "AddActuator ";
            if(strstr(groupString, keyword))
            {
                position = strstr(groupString, keyword);
                position += strlen(keyword);
                
                // Copy the actuatorType into the node structure
                strncpy(group[free].actuators->nodeName, position, MAX_NAME_LENGTH - 1);
                group[free].actuators->nodeName[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

                // Copy the actuatorName into the node structure
                position += MAX_NAME_LENGTH;
                strncpy(group[free].actuators->actuatorType, position, MAX_NAME_LENGTH -1);
                group[free].actuators->actuatorType[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

                // Copy the actuatorState into the node structure
                position += MAX_NAME_LENGTH;
                strncpy(group[free].actuators->actuatorName, position, MAX_NAME_LENGTH -1);
                group[free].actuators->actuatorName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
            }

            group[free].groupFilled = '1';
        } else
        {
            printk("Group not available");
        }
    }
}

void createGroup(struct Group* group)
{

}