#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <QString>

class Actuator
{
public:
    Actuator(QString);
    QString getActuatorName() const;

private:
    QString actuatorName;
};

#endif // ACTUATOR_H
