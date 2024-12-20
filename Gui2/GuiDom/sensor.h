#ifndef SENSOR_H
#define SENSOR_H

#include <QString>

class Sensor
{
public:
    Sensor(QString);
    QString getSensorName() const;

private:
    QString sensorName;
};

#endif // SENSOR_H
