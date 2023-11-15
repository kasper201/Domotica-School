#include "node.h"

Node::Node()
{

}

Node::~Node()
{

}

//stores node name
void Node::setNodeName(QString nodeName)
{
    name = nodeName;
}

//stores sensor
void Node::addSensor(QString sensorType, QString sensorName)
{
    qDebug() << sensorType << " en " << sensorName;
    sensorHash.insert(sensorType, sensorName);
}

//stores actuator
void Node::addActuator(QString actuatorType, QString actuatorName)
{
    qDebug() << actuatorType << " en " << actuatorName;
    sensorHash.insert(actuatorType, actuatorName);
}

//gets node name
QString Node::getNodeName()
{
    return name;
}

//gets sensors
QString Node::getSensors(QString sensorType)
{
    return sensorHash.value(sensorType);
}

//gets actuators
QString Node::getActuators(QString actuatorType)
{
    return sensorHash.value(actuatorType);
}
