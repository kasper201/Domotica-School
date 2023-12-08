#ifndef MAIN_H_
#define MAIN_H_

#define MAX_NODES_ALLOWED 10                //Max nodes in one system
#define MAX_SENSOR_FOR_NODE 1               //Max sensors in one node
#define MAX_ACTUATOR_FOR_NODE 1             //Max actuators in one node
#define MAX_NAME_LENGTH 13                  //Max a name or type is allowed to be
#define MAX_INFO_STRING_LENGTH 150          //Max length of the string with all node info
#define MAX_BOOLEAN_NAME 6                  //Max length of the string for boolean values

#define MAX_GROUPS_ALLOWED 10       //Max number of groups allowed to be active at the same time
#define MAX_SENSORS_IN_GROUP 2      //Max number of sensors that can be added to a group
#define MAX_ACTUATORS_IN_GROUP 2    //Max number of actuators that can be added to a group

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

struct GroupSensor {
    char nodeName[MAX_NAME_LENGTH];
    char sensorType[MAX_NAME_LENGTH];
    char sensorName[MAX_NAME_LENGTH];
};

struct GroupActuator {
    char nodeName[MAX_NAME_LENGTH];
    char actuatorType[MAX_NAME_LENGTH];
    char actuatorName[MAX_NAME_LENGTH];
};

struct Group {
    char groupName[MAX_NAME_LENGTH];
    struct GroupSensor sensors[MAX_SENSORS_IN_GROUP];
    struct GroupActuator actuators[MAX_ACTUATORS_IN_GROUP];
};

#endif //MAIN_H_