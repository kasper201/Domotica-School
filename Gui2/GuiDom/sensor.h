#ifndef SENSOR_H
#define SENSOR_H

#include <QString>

class Sensor
{
public:
    Sensor(QString nodeName, QString sensorName);
    QString getSensorName() const;
    QString getNodeName() const;

private:
    QString sensorName;
    QString nodeName;
};

#endif // SENSOR_H
