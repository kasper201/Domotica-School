#ifndef NODEDATA_H_
#define NODEDATA_H_

#define MAX_NODES_ALLOWED 10               //Max nodes in one system
#define MAX_SENSOR_FOR_NODE 1               //Max sensors in one node
#define MAX_ACTUATOR_FOR_NODE 1             //Max actuators in one node
#define MAX_NAME_LENGTH 12                   //Max a name or type is allowed to be
#define MAX_INFO_STRING_LENGTH 150     //Max length of the string with all node info

/*
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
};*/

extern char nodes[MAX_NODES_ALLOWED][MAX_INFO_STRING_LENGTH];

void nodesToPc(char (*nodes)[MAX_INFO_STRING_LENGTH]);

#endif //NODEDATA_H_