#include "groupData.h"

#include <zephyr/sys/printk.h>
#include <string.h>

//Defines group strings
char groups[MAX_GROUPS_ALLOWED][MAX_INFO_STRING_LENGTH];

//Sends groups to application
void groupsToPc(struct Group *group)
{
    //printk("%s", groups[0]);
    printk("AddGroup %s ", group->groupName);
    printk("AddSensor %s %s %s ", group->sensors->nodeName, group->sensors->sensorType, group->sensors->sensorName);
    printk("AddActuator %s %s %s\n", group->actuators->nodeName, group->actuators->actuatorType, group->actuators->actuatorName);
}

//Add a node
void addGroup(struct Group *group, char* groupString)
{
    

    if(strstr(groupString, "AddGroup"))
    {
        char* keyword = "AddGroup ";
        char* position = strstr(groupString, keyword);

        // Move the pointer to the start of nodeName (8 characters after "AddGroup")
        position += strlen(keyword);

        // Copy the nodeName into the node structure
        strncpy(group->groupName, position, MAX_NAME_LENGTH - 1);
        group->groupName[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

        keyword = "AddSensor ";
        if(strstr(groupString, keyword))
        {
            position = strstr(groupString, keyword);
            position += strlen(keyword);
            
             // Copy the sensorType into the node structure
            strncpy(group->sensors->nodeName, position, MAX_NAME_LENGTH - 1);
            group->sensors->nodeName[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

            // Copy the sensorType into the group structure
            strncpy(group->sensors->sensorType, position, MAX_NAME_LENGTH - 1);
            group->sensors->sensorType[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

            // Copy the sensorName into the node structure
            position += MAX_NAME_LENGTH;
            strncpy(group->sensors->sensorName, position, MAX_NAME_LENGTH -1);
            group->sensors->sensorName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
        }

        keyword = "AddActuator ";
        if(strstr(groupString, keyword))
        {
            position = strstr(groupString, keyword);
            position += strlen(keyword);
            
            // Copy the actuatorType into the node structure
            strncpy(group->actuators->nodeName, position, MAX_NAME_LENGTH - 1);
            group->actuators->nodeName[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

            // Copy the actuatorName into the node structure
            position += MAX_NAME_LENGTH;
            strncpy(group->actuators->actuatorType, position, MAX_NAME_LENGTH -1);
            group->actuators->actuatorType[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

            // Copy the actuatorState into the node structure
            position += MAX_NAME_LENGTH;
            strncpy(group->actuators->actuatorName, position, MAX_NAME_LENGTH -1);
            group->actuators->actuatorName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
        }
    }
}