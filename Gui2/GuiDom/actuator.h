#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <QString>

class Actuator
{
public:
    Actuator(QString nodeName, QString actuatorName);
    QString getActuatorName() const;
    QString getNodeName() const;

private:
    QString actuatorName;
    QString nodeName;
};

#endif // ACTUATOR_H
