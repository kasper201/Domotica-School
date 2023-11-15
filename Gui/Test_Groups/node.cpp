#include "node.h"

Node::Node()
{

}

Node::~Node()
{

}

//stores node name
void Node::addNodeInstance(QString nodeName)
{
    NodeComponents newNode;
    nodeInstances.insert(nodeName, newNode);
}

//stores sensor
void Node::addSensor(QString nodeName, QString sensorType, QString sensorName)
{
    if (nodeInstances.contains(nodeName))
    {
        nodeInstances[nodeName].sensorHash.insert(sensorType, sensorName);
    }
}

//stores actuator
void Node::addActuator(QString nodeName, QString actuatorType, QString actuatorName)
{
    if (nodeInstances.contains(nodeName))
    {
        nodeInstances[nodeName].actuatorHash.insert(actuatorType, actuatorName);
    }
}

//gets node names
QStringList Node::getAllNodeNames() const
{
    QStringList allNodeNames;

    // Iterate through the QMap and collect all node names
    for (auto it = nodeInstances.begin(); it != nodeInstances.end(); ++it)
    {
        allNodeNames.append(it.key());
    }

    return allNodeNames;
}

//gets all sensors from one node
QStringList Node::getAllSensorNames(QString nodeName) const
{
    return nodeInstances[nodeName].sensorHash.values();
}

//gets all actuators from one node
QStringList Node::getAllActuatorNames(QString nodeName) const
{
    return nodeInstances[nodeName].actuatorHash.values();
}

//gets sensors
QString Node::getSensors(QString nodeName, QString sensorType)
{
    return nodeInstances[nodeName].sensorHash.value(sensorType);
}

//gets actuators
QString Node::getActuators(QString nodeName, QString actuatorType)
{
    return nodeInstances[nodeName].actuatorHash.value(actuatorType);
}
