#include "sensor.h"

Sensor::Sensor(QString node, QString sensor) : sensorName(sensor), nodeName(node) {}

QString Sensor::getSensorName() const
{
    return sensorName;
}

QString Sensor::getNodeName() const
{
    return nodeName;
}
