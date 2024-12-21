#include "node.h"

Node::Node(QString name) : nodeName(name)
{}

Node::~Node() {}

QString Node::getNodeName() const
{
    return nodeName;
}
QStringList Node::getNodeSensors() const
{
    QStringList sensorNames;
    for (const Sensor& sensor : sensors) {
        sensorNames.append(sensor.getSensorName());
    }
    return sensorNames;
}

QStringList Node::getNodeActuators() const
{
    QStringList actuatorNames;
    for (const Actuator& actuator : actuators) {
        actuatorNames.append(actuator.getActuatorName());
    }
    return actuatorNames;
}

void Node::addSensor(QString sensorName)
{
    sensors.append(Sensor(nodeName, sensorName));
}

void Node::addActuator(QString actuatorName)
{
    actuators.append(Actuator(nodeName, actuatorName));
}
