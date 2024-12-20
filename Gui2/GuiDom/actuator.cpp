#include "actuator.h"

Actuator::Actuator(QString actuators) : actuatorName(actuators) {}

QString Actuator::getActuatorName() const
{
    return actuatorName;
}
