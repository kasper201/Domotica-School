#include "nodes.h"

Nodes::Nodes(Domotica* domotica, QObject* parent) : UIdomotica(domotica)
{}

Nodes::~Nodes() {}

void Nodes::addNode(QString nodeName)
{
    bool alreadyExist = false;
    for (const Node& node : nodes) {
        if(node.getNodeName() == nodeName)
        {
            alreadyExist = true;
        }
    }
    if(!alreadyExist)
    {
        nodes.append(Node(nodeName));
        UIdomotica->GetNodeList()->addItem(nodeName);
        qDebug() << "Node added: " << nodeName;
    }
    else
    {
        qDebug() << "Node already exists";
    }
}

void Nodes::addSensorToNode(QString nodeName, QString sensorName)
{
    for (Node& node : nodes) {
        if(node.getNodeName() == nodeName)
        {
            bool alreadyExist = false;
            QStringList sensorList = node.getNodeSensors();
            for (const QString& sensor : sensorList) {
                if(sensor == sensorName)
                {
                    alreadyExist = true;
                }
            }
            if(!alreadyExist)
            {
                node.addSensor(sensorName);
                qDebug() << "Sensor: " << sensorName << " Has been added to: " << nodeName;
            }
            else
            {
                qDebug() << "Sensor already exists for this node";
            }
        }
    }
}

void Nodes::addActuatorToNode(QString nodeName, QString actuatorName)
{
    for (Node& node : nodes) {
        if(node.getNodeName() == nodeName)
        {
            bool alreadyExist = false;
            QStringList actuatorList = node.getNodeActuators();
            for (const QString& actuator : actuatorList) {
                if(actuator == actuatorName)
                {
                    alreadyExist = true;
                }
            }
            if(!alreadyExist)
            {
                node.addActuator(actuatorName);
                qDebug() << "Actuator: " << actuatorName << " Has been added to: " << nodeName;
            }
            else
            {
                qDebug() << "Actuator already exists for this node";
            }
        }
    }
}

QStringList Nodes::getNodeNames()
{
    QStringList nodeNames;
    for (const Node& node : nodes) {
        nodeNames.append(node.getNodeName());
    }
    return nodeNames;
}

void Nodes::showParts(QString nodeName)
{
    for (Node& node : nodes) {
        if(node.getNodeName() == nodeName)
        {
            QStringList sensorList = node.getNodeSensors();
            UIdomotica->GetNodeSensorsList()->clear();
            for (const QString& sensor : sensorList) {
                UIdomotica->GetNodeSensorsList()->addItem(sensor);
            }
            QStringList actuatorList = node.getNodeActuators();
            UIdomotica->GetNodeActuatorsList()->clear();
            for (const QString& actuator : actuatorList) {
                 UIdomotica->GetNodeActuatorsList()->addItem(actuator);
            }
        }
    }
}
