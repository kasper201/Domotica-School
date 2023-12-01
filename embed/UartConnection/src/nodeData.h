#ifndef NODEDATA_H_
#define NODEDATA_H_

#define MAX_NODES_ALLOWED 100               //Max nodes in one system
#define MAX_SENSOR_FOR_NODE 2               //Max sensors in one node
#define MAX_ACTUATOR_FOR_NODE 2             //Max actuators in one node
#define MAX_NAME_LENGTH 12                   //Max a name or type is allowed to be
#define MAX_NODE_INFO_STRING_LENGTH 200     //Max length of the string with all node info

struct Sensor {
    char sensorName[MAX_NAME_LENGTH];
    char sensorType[MAX_NAME_LENGTH];
};

struct Actuator {
    char actuatorName[MAX_NAME_LENGTH];
    char actuatorType[MAX_NAME_LENGTH];
    char actuatorState[5];
};

struct Node {
    char nodeName[MAX_NAME_LENGTH];
    struct Sensor sensor[MAX_SENSOR_FOR_NODE];
    struct Actuator actuator[MAX_ACTUATOR_FOR_NODE];
};

extern char nodes[MAX_NODES_ALLOWED][MAX_NODE_INFO_STRING_LENGTH];

void nodesToPc(char (*nodes)[MAX_NODE_INFO_STRING_LENGTH]);

#endif //NODEDATA_H_