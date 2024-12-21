#include "groups.h"

Groups::Groups(Domotica* domotica, Nodes* nodes,  QObject* parent) : QObject(parent), UIdomotica(domotica)
{
}

Groups::~Groups(){}

//Add a new Group
void Groups::AddGroup(QString groupName)
{
    if(!groupName.isEmpty())
    {
        bool alreadyExist = false;
        for (const QString& groupNameError : groupsMap.keys()) //checks if the group already exists
        {
            if(groupName == groupNameError)
            {
                alreadyExist = true;
            }
        }
        if(!alreadyExist)
        {
            qDebug() << "Add: " << groupName;
            groupParts newGroupParts;
            groupsMap.insert(groupName, newGroupParts);
            UIdomotica->GetGroupList()->addItem(groupName);
            UIdomotica->GetGroupName()->clear();
        }
        else
        {
            qDebug() << "Group already exists";
            UIdomotica->GetGroupName()->clear();
        }
    }
    else
    {
        qDebug() << "Nothing to add";
    }
}

//Delete a Group
void Groups::DeleteGroup(QString groupName)
{
    if(!UIdomotica->GetGroupList()->selectedItems().isEmpty())
    {
        UIdomotica->GetGroupList()->takeItem(UIdomotica->GetGroupList()->currentRow());
        groupsMap.remove(groupName);
        qDebug() << "Deleted: " << groupName;
    }
    else
    {
    qDebug() << "Select a Group to delete";
    }
}

//Add a sensor to the group
void Groups::AddSensor(QString groupName, QString nodeName, QString sensorName)
{
    if(!groupName.isEmpty() && !nodeName.isEmpty() && !sensorName.isEmpty())
    {
        bool alreadyExist = false;
        // Check if the sensor is already in the sensorList for the specified groupName
        if (groupsMap.contains(groupName)) {
                const groupParts &group = groupsMap[groupName];
                alreadyExist = group.sensorList.contains(nodeName, sensorName);
        }

        if (!alreadyExist) {
                // Add the sensor to the sensorList
                groupsMap[groupName].sensorList.insert(nodeName, sensorName);
                qDebug() << "Sensor added successfully to group: " << groupName;
        } else {
                qDebug() << "Sensor already exists in group: " << groupName;
        }
        showGroup(groupName);
    }
    else
    {
    qDebug() << "Not everything has been selected";
    }
}

//Add a actuator to the group
void Groups::AddActuator(QString groupName, QString nodeName, QString actuatorName)
{
    if(!groupName.isEmpty() && !nodeName.isEmpty() && !actuatorName.isEmpty())
    {
        bool alreadyExist = false;
        // Check if the sensor is already in the sensorList for the specified groupName
        if (groupsMap.contains(groupName)) {
                    const groupParts &group = groupsMap[groupName];
                    alreadyExist = group.actuatorList.contains(nodeName, actuatorName);
        }

        if (!alreadyExist) {
                    // Add the sensor to the sensorList
                    groupsMap[groupName].actuatorList.insert(nodeName, actuatorName);
                    qDebug() << "Actuator added successfully to group: " << groupName;
        } else {
                    qDebug() << "Actuator already exists in group: " << groupName;
        }
        showGroup(groupName);
    }
    else
    {
        qDebug() << "Not everything has been selected";
    }
}

void Groups::handleGroupAdd()
{
    AddGroup(UIdomotica->GetGroupName()->text());
}

void Groups::handleGroupDelete()
{
    DeleteGroup(UIdomotica->GetGroupList()->currentItem()->text());
}

void Groups::handleSensorAdd()
{
    QString groupName = UIdomotica->GetGroupList()->currentItem()->text();
    QString nodeName = UIdomotica->GetNodeList()->currentItem()->text();
    QString sensorName = UIdomotica->GetNodeSensorsList()->currentItem()->text();
    AddSensor(groupName, nodeName, sensorName);
}

void Groups::handleActuatorAdd()
{
    QString groupName = UIdomotica->GetGroupList()->currentItem()->text();
    QString nodeName = UIdomotica->GetNodeList()->currentItem()->text();
    QString actuatorName = UIdomotica->GetNodeActuatorsList()->currentItem()->text();
    AddActuator(groupName, nodeName, actuatorName);
}

void Groups::showGroup(QString groupName)
{
    if (groupsMap.contains(groupName)) {
    // Get the groupParts for the specified groupName
    const groupParts &group = groupsMap[groupName];

    UIdomotica->GetGroupSensorsList()->clear();
    UIdomotica->GetGroupActuatorsList()->clear();

    // Add all sensors to the QListWidget
    for (auto it = group.sensorList.constBegin(); it != group.sensorList.constEnd(); ++it) {
                QString nodeName = it.key();
                QString sensorName = it.value();
                UIdomotica->GetGroupSensorsList()->addItem(nodeName + " | " + sensorName);
    }

    // Add all actuators to the QListWidget
    for (auto it = group.actuatorList.constBegin(); it != group.actuatorList.constEnd(); ++it) {
                QString nodeName = it.key();
                QString actuatorName = it.value();
                UIdomotica->GetGroupActuatorsList()->addItem(nodeName + " | " + actuatorName);
    }
    }
    else {
    qDebug() << "Group" << groupName << "does not exist.";
    }
}

