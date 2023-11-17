#ifndef SENSOR_H
#define SENSOR_H

#include "groups.h"

class Sensor
{
public:
    Sensor(Groups &groups);
    ~Sensor();

    void sensorTrigger(QString data);
};

#endif // SENSOR_H
