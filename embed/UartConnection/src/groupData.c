#include "groupData.h"

#include <zephyr/sys/printk.h>
#include <string.h>
#include <zephyr/kernel.h>

// Defines group strings
char groups[MAX_GROUPS_ALLOWED][MAX_INFO_STRING_LENGTH];

// Empty max string
char EMPTY_MAX_NAME[MAX_NAME_LENGTH];

// MAX name empty string
void createEmptyMax()
{
    for (int i = 0; i < MAX_NAME_LENGTH; i++)
    {
        if (i < MAX_NAME_LENGTH - 1)
        {
            EMPTY_MAX_NAME[i] = '_';
        }
        else
        {
            EMPTY_MAX_NAME[i] = '\0';
        }
    }
}

// Sends groups to application
void groupsToPc(struct Group *group)
{
    // printk("%s", groups[0]);
    for (int i = 0; i < MAX_GROUPS_ALLOWED; i++)
    {
        if (group[i].groupFilled == '1')
        {
            printk("AddGroup %s ", group[i].groupName);
            printk("AddSensor %s %s %s ", group[i].sensors->nodeName, group[i].sensors->sensorType, group[i].sensors->sensorName);
            printk("AddActuator %s %s %s\n", group[i].actuators->nodeName, group[i].actuators->actuatorType, group[i].actuators->actuatorName);
            k_msleep(5);
        }
    }
}

// Add a Group
void addGroup(struct Group *group, char *groupString)
{
    int free = 0;
    char filled = '0';
    while (group[free].groupFilled == '1' && free < MAX_GROUPS_ALLOWED)
    {
        if (strstr(groupString, group[free].groupName))
        {
            filled = '1';
            break;
        }

        free++;
    }

    if (group[free].groupFilled == '0' && filled != '1')
    {
        if (strstr(groupString, "AddGroup"))
        {
            char *keyword = "AddGroup ";
            char *position = strstr(groupString, keyword);

            // Move the pointer to the start of groupName (8 characters after "AddGroup")
            position += strlen(keyword);

            // Copy the groupName into the group structure
            strncpy(group[free].groupName, position, MAX_NAME_LENGTH - 1);
            group[free].groupName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

            // Add a new sensor to the group
            keyword = "AddSensor ";
            if (strstr(groupString, keyword))
            {
                int sensorFree = 0;
                while (group[free].sensors[sensorFree].sensorFilled != '0' && sensorFree < MAX_SENSORS_IN_GROUP)
                {
                    sensorFree++;
                }

                if (sensorFree < MAX_SENSORS_IN_GROUP)
                {
                    position = strstr(groupString, keyword);
                    position += strlen(keyword);

                    // Copy the sensorType into the node structure
                    strncpy(group[free].sensors[sensorFree].nodeName, position, MAX_NAME_LENGTH - 1);
                    group[free].sensors[sensorFree].nodeName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

                    // Copy the sensorType into the group structure
                    position += MAX_NAME_LENGTH;
                    strncpy(group[free].sensors[sensorFree].sensorType, position, MAX_NAME_LENGTH - 1);
                    group[free].sensors[sensorFree].sensorType[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

                    // Copy the sensorName into the node structure
                    position += MAX_NAME_LENGTH;
                    strncpy(group[free].sensors[sensorFree].sensorName, position, MAX_NAME_LENGTH - 1);
                    group[free].sensors[sensorFree].sensorName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

                    group[free].sensors[sensorFree].sensorFilled = '1';
                }
            }

            // Add a new actuator to the group
            keyword = "AddActuator ";
            if (strstr(groupString, keyword))
            {
                position = strstr(groupString, keyword);
                position += strlen(keyword);

                // Copy the actuatorType into the node structure
                strncpy(group[free].actuators[0].nodeName, position, MAX_NAME_LENGTH - 1);
                group[free].actuators[0].nodeName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

                // Copy the actuatorName into the node structure
                position += MAX_NAME_LENGTH;
                strncpy(group[free].actuators[0].actuatorType, position, MAX_NAME_LENGTH - 1);
                group[free].actuators[0].actuatorType[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

                // Copy the actuatorState into the node structure
                position += MAX_NAME_LENGTH;
                strncpy(group[free].actuators[0].actuatorName, position, MAX_NAME_LENGTH - 1);
                group[free].actuators[0].actuatorName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

                group[free].actuators[0].actuatorFilled = '1';
            }

            group[free].groupFilled = '1';
        }
        else
        {
            printk("Group not available");
        }
    }
}

// Create a new group
void createGroup(struct Group *group, char *groupString)
{
    // Check for the first availble slot for the new group
    int free = 0;
    char filled = '0';
    while (group[free].groupFilled == '1' && free < MAX_GROUPS_ALLOWED)
    {
        if (strstr(groupString, group[free].groupName))
        {
            filled = '1';
            break;
        }

        free++;
    }

    // Enter the new group name
    if (group[free].groupFilled == '0' && filled != '1')
    {
        if (strstr(groupString, "CreateGroup"))
        {
            char *keyword = "CreateGroup ";
            char *position = strstr(groupString, keyword);

            // Move the pointer to the start of groupName (8 characters after "AddGroup")
            position += strlen(keyword);

            // Copy the groupName into the group structure
            strncpy(group[free].groupName, position, MAX_NAME_LENGTH - 1);
            group[free].groupName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
            group[free].groupFilled = '1';

            //Fills the empty string with '_"
            createEmptyMax();

            // Fills all sensor data with empty info
            for (int i = 0; i < MAX_SENSORS_IN_GROUP; i++)
            {
                strcpy(group[free].sensors[i].nodeName, EMPTY_MAX_NAME);
                strcpy(group[free].sensors[i].sensorType, EMPTY_MAX_NAME);
                strcpy(group[free].sensors[i].sensorName, EMPTY_MAX_NAME);
            }

            // Fills all actuator data with empty info
            for (int i = 0; i < MAX_ACTUATORS_IN_GROUP; i++)
            {
                strcpy(group[free].actuators[i].nodeName, EMPTY_MAX_NAME);
                strcpy(group[free].actuators[i].actuatorType, EMPTY_MAX_NAME);
                strcpy(group[free].actuators[i].actuatorName, EMPTY_MAX_NAME);
            }
        }
    }
    else
    {
        // Group already exist
        // printk("This group already exist");
    }
}

// Add sensor or actuator
void updateGroup(struct Group *group, char *groupString)
{
    int check = 0;
    while (!strstr(groupString, group[check].groupName) && check < MAX_GROUPS_ALLOWED)
    {
        check++;
    }

    // Add a sensor
    char *keyword = "AddSensor ";
    char *position = strstr(groupString, keyword);
    if (strstr(groupString, keyword))
    {
        int sensorFree = 0;
        while (group[check].sensors[sensorFree].sensorFilled != '0' && sensorFree < MAX_SENSORS_IN_GROUP)
        {
            sensorFree++;
        }

        if (sensorFree < MAX_SENSORS_IN_GROUP)
        {
            position = strstr(groupString, keyword);
            position += strlen(keyword);

            // Copy the sensorType into the node structure
            strncpy(group[check].sensors[sensorFree].nodeName, position, MAX_NAME_LENGTH - 1);
            group[check].sensors[sensorFree].nodeName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

            // Copy the sensorType into the group structure
            position += MAX_NAME_LENGTH;
            strncpy(group[check].sensors[sensorFree].sensorType, position, MAX_NAME_LENGTH - 1);
            group[check].sensors[sensorFree].sensorType[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

            // Copy the sensorName into the node structure
            position += MAX_NAME_LENGTH;
            strncpy(group[check].sensors[sensorFree].sensorName, position, MAX_NAME_LENGTH - 1);
            group[check].sensors[sensorFree].sensorName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

            group[check].sensors[sensorFree].sensorFilled = '1';
        }
    }
}