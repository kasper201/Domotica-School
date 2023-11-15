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
    qDebug() << nodeName;
    nodeInstances.insert(nodeName, newNode);
    qDebug() << nodeInstances.count(); //returns 1
}

//stores sensor
void Node::addSensor(QString nodeName, QString sensorType, QString sensorName)
{
    qDebug() << nodeName;
    qDebug() << nodeInstances.count(); //returns 0 while it is called after addNodeInstance
    if (nodeInstances.contains(nodeName))
    {
        qDebug() << sensorType << " and " << sensorName;
        qDebug() << nodeName;
        nodeInstances[nodeName].sensorHash.insert(sensorType, sensorName);
    }
}

//stores actuator
void Node::addActuator(QString nodeName, QString actuatorType, QString actuatorName)
{
    if (nodeInstances.contains(nodeName))
    {
        qDebug() << actuatorType << " and " << actuatorName;
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
