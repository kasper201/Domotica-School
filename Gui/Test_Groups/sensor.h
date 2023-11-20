#ifndef SENSOR_H
#define SENSOR_H

#include <QDebug>
#include <QString>

#include "groups.h"
#include "node.h"
#include "stringmodifiers.h"

class Sensor
{
public:
    Sensor();
    ~Sensor();

    QStringList sensorTrigger(Groups& groups, QString data);           //sees if a sensor is triggered and to which group(s) it is linked
    QStringList groupTriggered(Groups& groups, Node& node, QString groupName);     //will trigger the actuators belonging to the triggered group
    void actuatorUpdate(Node& node, QString data);                                              //Updates status off actuator

private:
    StringModifiers stringM;
    QStringList AllUpdates;
};

#endif // SENSOR_H
