#ifndef SENSOR_H
#define SENSOR_H

#include <QDebug>
#include <QString>

#include "groups.h"
#include "portsetup.h"
#include "node.h"
#include "stringmodifiers.h"

class Sensor
{
public:
    Sensor();
    ~Sensor();

    void sensorTrigger(Groups& groups, Node& node, PortSetup& comport, QString data);           //sees if a sensor is triggered and to which group(s) it is linked
    void groupTriggered(Groups& groups, Node& node, PortSetup& comport, QString groupName);     //will trigger the actuators belonging to the triggered group
    void actuatorUpdate(Node& node, QString data);                                              //Updates status off actuator

private:
    StringModifiers stringM;
};

#endif // SENSOR_H
