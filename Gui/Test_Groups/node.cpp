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
void Node::addActuator(QString nodeName, QString actuatorType, QString actuatorName, QString actuatorStatus)
{
    if (nodeInstances.contains(nodeName))
    {
        nodeInstances[nodeName].actuatorHash.insert(actuatorType, actuatorName);
        if(actuatorStatus == "true")                                                    //enters true as status
        {
            nodeInstances[nodeName].actuatorState.insert(actuatorName, true);
        } else if (actuatorStatus == "false")                                           //enters false as status
        {
            nodeInstances[nodeName].actuatorState.insert(actuatorName, false);
        } else                                                                          //error handeling
        {
            qDebug() << "Not an accepted state : " << actuatorStatus;
        }

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
    QStringList sensorInfo;
    for (const QString& sensorType : nodeInstances[nodeName].sensorHash.uniqueKeys())
    {
        for (const QString& sensorName : nodeInstances[nodeName].sensorHash.values(sensorType))
        {
            QString sensorString = sensorName + '\t' + sensorType;
            sensorInfo.append(sensorString);
        }
    }

    return sensorInfo;
}

//gets all actuators from one node
QStringList Node::getAllActuatorNames(QString nodeName) const
{
    QStringList actuatorInfo;
    for (const QString& actuatorType : nodeInstances[nodeName].actuatorHash.uniqueKeys())
    {
        for (const QString& actuatorName : nodeInstances[nodeName].actuatorHash.values(actuatorType))
        {
            QString actuatorString = actuatorName + '\t' + actuatorType;
            actuatorInfo.append(actuatorString);
        }
    }

    return actuatorInfo;
}

bool Node::getActuatorStatus(QString nodeName, QString actuatorName)
{
    return nodeInstances[nodeName].actuatorState.value(actuatorName);
}
