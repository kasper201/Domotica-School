#include "nodes.h"

Nodes::Nodes(Domotica* domotica, QObject* parent) : UIdomotica(domotica)
{}

Nodes::~Nodes() {}

void Nodes::addNode(QString nodeName, int nodeAddress)
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
        nodes.append(Node(nodeName, nodeAddress));
        UIdomotica->GetNodeList()->addItem(nodeName);
        qDebug() << "Node added: " << nodeName << " Node address: " << nodeAddress;
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

int Nodes::FirstFreeNodeAddress()
{
    int nodeAddress = 1;

    while (true) {
        bool found = false;
        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            if (it->getNodeAddress() == nodeAddress) {
                found = true;
                break;
            }
        }

        if (!found) {
            qDebug() << "First free nodeAddres is: " << nodeAddress;
            return nodeAddress; // This is the first empty group address
        }

        for(int i = 0; i < NODE_ADDRESS_INCREMENTS; i++) // Increment to check the next address
        {
            ++nodeAddress;
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

int Nodes::getNodeAddress(QString nodeName)
{
    for (Node& node : nodes) {
        if(node.getNodeName() == nodeName)
        {
            return node.getNodeAddress();
        }
    }
    return -1;
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
