#include "groups.h"

Groups::Groups(Domotica* domotica,  QObject* parent) : QObject(parent), UIdomotica(domotica)
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
            groupsMap[groupName].groupAddress = FirstFreeGroupAddress();
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

int Groups::FirstFreeGroupAddress()
{
    int groupAddress = 49152;

    while (true) {
    bool found = false;
    for (auto it = groupsMap.begin(); it != groupsMap.end(); ++it) {
            if (it.value().groupAddress == groupAddress) {
                found = true;
                break;
            }
    }

    if (!found) {
            qDebug() << "GroupAddres is: " << groupAddress;
            return groupAddress; // This is the first empty group address
    }

    ++groupAddress; // Increment to check the next address
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
        } else {
                qDebug() << "First select a group";
                return;
        }

        if (!alreadyExist ) {
                // Add the sensor to the sensorList
                groupsMap[groupName].sensorList.insert(nodeName, sensorName);
                qDebug() << "Sensor added successfully to group: " << groupName;
        } else {
                qDebug() << "Sensor already exists in group: " << groupName;
        }
        showGroup(groupName);
        subscribeToGroup(groupName, nodeName, true);
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
        } else {
                    qDebug() << "First select a group";
                    return;
        }

        if (!alreadyExist) {
                    // Add the sensor to the sensorList
                    groupsMap[groupName].actuatorList.insert(nodeName, actuatorName);
                    qDebug() << "Actuator added successfully to group: " << groupName;
        } else {
                    qDebug() << "Actuator already exists in group: " << groupName;
        }
        showGroup(groupName);
        subscribeToGroup(groupName, nodeName, false);
    }
    else
    {
        qDebug() << "Not everything has been selected";
    }
}

void Groups::DeleteSensor(QString groupName, QString nodeName, QString sensorName)
{
    if(!groupName.isEmpty() && !nodeName.isEmpty() && !sensorName.isEmpty())
    {
        bool alreadyExist = false;
        // Check if the sensor is in the sensorList for the specified groupName
        if (groupsMap.contains(groupName)) {
                    const groupParts &group = groupsMap[groupName];
                    alreadyExist = group.sensorList.contains(nodeName, sensorName);
        } else {
                    qDebug() << "First select a group";
                    return;
        }

        if (alreadyExist ) {
                    // Delete the sensor to the sensorList
                    groupsMap[groupName].sensorList.remove(nodeName, sensorName);
                    qDebug() << "Sensor successfully deleted from group: " << groupName;
        } else {
                    qDebug() << "The sensor is not in group: " << groupName;
        }
        showGroup(groupName);
        unsubscribeFromGroup(groupName, nodeName, true);
    }
    else
    {
        qDebug() << "Not everything has been selected";
    }
}

void Groups::DeleteActuator(QString groupName, QString nodeName, QString actuatorName)
{
    if(!groupName.isEmpty() && !nodeName.isEmpty() && !actuatorName.isEmpty())
    {
        bool alreadyExist = false;
        // Check if the actuator is in the actuatorList for the specified groupName
        if (groupsMap.contains(groupName)) {
                    const groupParts &group = groupsMap[groupName];
                    alreadyExist = group.actuatorList.contains(nodeName, actuatorName);
        } else {
                    qDebug() << "First select a group";
                    return;
        }

        if (alreadyExist ) {
                    // Delete the actuator from the actuatorList
                    groupsMap[groupName].actuatorList.remove(nodeName, actuatorName);
                    qDebug() << "Actuator successfully deleted from group: " << groupName;
        } else {
                    qDebug() << "The actuator is not in group: " << groupName;
        }
        showGroup(groupName);
        unsubscribeFromGroup(groupName, nodeName, false);
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
    //Check if everything has been selected
    if (UIdomotica->GetGroupList()->currentRow() == -1)
    {
        qDebug() << "No group has been selected";
        return;
    } else if (UIdomotica->GetNodeList()->currentRow() == -1)
    {
        qDebug() << "No node has been selected";
        return;
    } else if (UIdomotica->GetNodeSensorsList()->currentRow() == -1)
    {
        qDebug() << "No sensor has been selected";
        return;
    }
    //save the names
    QString groupName = UIdomotica->GetGroupList()->currentItem()->text();
    QString nodeName = UIdomotica->GetNodeList()->currentItem()->text();
    QString sensorName = UIdomotica->GetNodeSensorsList()->currentItem()->text();

    //add the sensor
    AddSensor(groupName, nodeName, sensorName);
}

void Groups::handleActuatorAdd()
{
    //Check if everything has been selected
    if (UIdomotica->GetGroupList()->currentRow() == -1)
    {
        qDebug() << "No group has been selected";
        return;
    } else if (UIdomotica->GetNodeList()->currentRow() == -1)
    {
        qDebug() << "No node has been selected";
        return;
    } else if (UIdomotica->GetNodeActuatorsList()->currentRow() == -1)
    {
        qDebug() << "No actuator has been selected";
        return;
    }
    //save the names
    QString groupName = UIdomotica->GetGroupList()->currentItem()->text();
    QString nodeName = UIdomotica->GetNodeList()->currentItem()->text();
    QString actuatorName = UIdomotica->GetNodeActuatorsList()->currentItem()->text();

    //add the actuator
    AddActuator(groupName, nodeName, actuatorName);
}

void Groups::handleSensorDelete()
{
    //Check if everything has been selected
    if (UIdomotica->GetGroupList()->currentRow() == -1)
    {
        qDebug() << "No group has been selected";
        return;
    } else if (UIdomotica->GetGroupSensorsList()->currentRow() == -1)
    {
        qDebug() << "No sensor has been selected";
        return;
    }

    //save the names
    QString groupName = UIdomotica->GetGroupList()->currentItem()->text();
    QStringList sensorInfo = UIdomotica->GetGroupSensorsList()->currentItem()->text().split(" | ");
    QString nodeName = sensorInfo[0];
    QString sensorName = sensorInfo[1];
    DeleteSensor(groupName, nodeName, sensorName);
}

void Groups::handleActuatorDelete()
{
    //Check if everything has been selected
    if (UIdomotica->GetGroupList()->currentRow() == -1)
    {
        qDebug() << "No group has been selected";
        return;
    } else if (UIdomotica->GetGroupActuatorsList()->currentRow() == -1)
    {
        qDebug() << "No actuator has been selected";
        return;
    }

    //save the names
    QString groupName = UIdomotica->GetGroupList()->currentItem()->text();
    QStringList actuatorInfo = UIdomotica->GetGroupActuatorsList()->currentItem()->text().split(" | ");
    QString nodeName = actuatorInfo[0];
    QString actuatorName = actuatorInfo[1];
    DeleteActuator(groupName, nodeName, actuatorName);
}

void Groups::handleComputerToGroups()
{
    for (auto it = groupsMap.begin(); it != groupsMap.end(); ++it) {
        groupParts &group = it.value();
        if (group.sensorList.contains(COMPUTER_NODE_NAME, STRING_TO_SHOW_BUTTON + QString(NODE_SHOW) + COMPUTER_NODE_NAME)) {
            group.groupState = !group.groupState;
            emit sendToGroup(group.groupAddress, group.groupState);
            handleGroupsToComputer(group.groupAddress, group.groupState);
            qDebug() << "Group activated: " << it.key() << " Updated to state: " << group.groupState;
        }
    }
}

void Groups::handleGroupsToComputer(int groupAddress, bool newState)
{
    for (auto it = groupsMap.begin(); it != groupsMap.end(); ++it) {
        groupParts &group = it.value();
        if(group.groupAddress == groupAddress)
        {
            group.groupState = newState;
            if (group.actuatorList.contains(COMPUTER_NODE_NAME, STRING_TO_SHOW_LED + QString(NODE_SHOW) + COMPUTER_NODE_NAME)) {
                if(newState) {
                    UIdomotica->GetComputerNode()->setStyleSheet("background-color: yellow;");
                } else {
                    UIdomotica->GetComputerNode()->setStyleSheet("background-color: black;");
                }
            }
        }
    }
}

int Groups::GetGroupAddress(QString groupName)
{
    return groupsMap[groupName].groupAddress;
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

