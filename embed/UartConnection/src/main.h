#ifndef MAIN_H_
#define MAIN_H_

#define MAX_NODES_ALLOWED 10                //Max nodes in one system
#define MAX_SENSOR_FOR_NODE 1               //Max sensors in one node
#define MAX_ACTUATOR_FOR_NODE 1             //Max actuators in one node
#define MAX_NAME_LENGTH 13                  //Max a name or type is allowed to be
#define MAX_INFO_STRING_LENGTH 150          //Max length of the string with all node info
#define MAX_BOOLEAN_NAME 6                  //Max length of the string for boolean values

#define MAX_GROUPS_ALLOWED 10       //Max number of groups allowed to be active at the same time

struct Sensor {
    char sensorName[MAX_NAME_LENGTH];
    char sensorType[MAX_NAME_LENGTH];
};

struct Actuator {
    char actuatorName[MAX_NAME_LENGTH];
    char actuatorType[MAX_NAME_LENGTH];
    char actuatorState[MAX_BOOLEAN_NAME];
};

struct Node {
    char nodeName[MAX_NAME_LENGTH];
    struct Sensor sensor[MAX_SENSOR_FOR_NODE];
    struct Actuator actuator[MAX_ACTUATOR_FOR_NODE];
};

struct Group {
    char groupName[MAX_NAME_LENGTH];
};

#endif //MAIN_H_