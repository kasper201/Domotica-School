#include "nodeData.h"
#include "main.h"

#include <zephyr/sys/printk.h>
#include <string.h>

//Defines char nodes
char nodes[MAX_NODES_ALLOWED][MAX_INFO_STRING_LENGTH];

//Sends all nodes currently connected to the pc
void nodesToPc(struct Node* node)
{
    printk("AddNode %s ", node->nodeName);
    printk("AddSensor %s %s ", node->sensor->sensorType, node->sensor->sensorName);
    printk("AddActuator %s %s %s\n", node->actuator->actuatorType, node->actuator->actuatorName, node->actuator->actuatorState);
}

//Add a node
void addNode(struct Node *node, char* nodeString)
{
    

    if(strstr(nodeString, "AddNode"))
    {
        char* keyword = "AddNode ";
        char* position = strstr(nodeString, keyword);

        // Move the pointer to the start of nodeName (8 characters after "AddNode")
        position += strlen(keyword);

        // Copy the nodeName into the node structure
        strncpy(node->nodeName, position, MAX_NAME_LENGTH - 1);
        node->nodeName[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

        keyword = "AddSensor ";
        if(strstr(nodeString, keyword))
        {
            position = strstr(nodeString, keyword);
            position += strlen(keyword);
            
            // Copy the sensorType into the node structure
            strncpy(node->sensor->sensorType, position, MAX_NAME_LENGTH - 1);
            node->sensor->sensorType[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

            // Copy the sensorName into the node structure
            position += MAX_NAME_LENGTH;
            strncpy(node->sensor->sensorName, position, MAX_NAME_LENGTH -1);
            node->sensor->sensorName[MAX_NAME_LENGTH - 1] = '\0';
        }

        keyword = "AddActuator ";
        if(strstr(nodeString, keyword))
        {
            position = strstr(nodeString, keyword);
            position += strlen(keyword);
            
            // Copy the actuatorType into the node structure
            strncpy(node->actuator->actuatorType, position, MAX_NAME_LENGTH - 1);
            node->actuator->actuatorType[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

            // Copy the actuatorName into the node structure
            position += MAX_NAME_LENGTH;
            strncpy(node->actuator->actuatorName, position, MAX_NAME_LENGTH -1);
            node->actuator->actuatorName[MAX_NAME_LENGTH - 1] = '\0';

            // Copy the actuatorState into the node structure
            position += MAX_NAME_LENGTH;
            strncpy(node->actuator->actuatorState, position, MAX_BOOLEAN_NAME -1);
            node->actuator->actuatorState[MAX_BOOLEAN_NAME - 1] = '\0';
        }
    }
}