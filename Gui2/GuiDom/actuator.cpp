#include "actuator.h"

Actuator::Actuator(QString node, QString actuator) : actuatorName(actuator), nodeName(node) {}

QString Actuator::getActuatorName() const
{
    return actuatorName;
}

QString Actuator::getNodeName() const
{
    return nodeName;
}
