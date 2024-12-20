#include "sensor.h"

Sensor::Sensor(QString sensor) : sensorName(sensor) {}

QString Sensor::getSensorName() const
{
    return sensorName;
}
