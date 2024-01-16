#include "groups.h"

Groups::Groups()
{

}

Groups::~Groups()
{

}

//Adds a new group
void Groups::addGroupInstance(QString groupName)
{
    bool alreadyExist = false;
    QStringList groupKeys = groupMap.keys();
    for(const QString& groupNameError : groupKeys)
    {
        if(groupNameError == groupName)
        {
            alreadyExist = true;
        }
    }
    if(alreadyExist == false)
    {
        groupParts newGroupParts;
        groupMap.insert(groupName, newGroupParts);
        for (int address = startValueAddress; address < (startValueAddress + maxGroupAddress); address++)
        {
            if(!groupAddress.values().contains(address))
            {
                groupAddress.insert(groupName, address);
                break;
            }
        }
        qDebug() << "Group: " << groupName << " has been added";
    } else
    {
        qDebug() << "Group: " << groupName << " already exists";
    }
}

//Adds a sensor to a group
void Groups::addSensor(QString groupName, QString nodeName, QString sensorType, QString sensorName)
{
    bool alreadyExist = false;
    for (const QString& sensorNameError : groupMap[groupName].sensorsInGroup.uniqueKeys()) //checks if the sensor is already added to the group
    {
        if(sensorName == sensorNameError)
        {
            alreadyExist = true;
        }
    }
    if(alreadyExist == false)                                                               //Adds sensor if it is not present yet
    {
        sensorInfo = qMakePair(sensorName, nodeName);
        sensorGroupLink.insert(sensorInfo, groupName);
        sensorInfo = qMakePair(nodeName, sensorType);
        groupMap[groupName].sensorsInGroup.insert(sensorName, sensorInfo);
    } else                                                                                  //Gives qDebug that sensor has already been added
    {
        qDebug() << "This sensor has already been added";
    }
}

//Adds a actuator to a group
void Groups::addActuator(QString groupName, QString nodeName, QString actuatorType, QString actuatorName)
{
    bool alreadyExist = false;
    for (const QString& actuatorNameError : groupMap[groupName].actuatorsInGroup.uniqueKeys()) //checks if the actuator is already added to the group
    {
        if(actuatorName == actuatorNameError)
        {
            alreadyExist = true;
        }
    }
    if(alreadyExist == false)                                                               //Adds actuator if it is not present yet
    {
        actuatorInfo = qMakePair(actuatorName, nodeName);
        actuatorGroupLink.insert(actuatorInfo, groupName);
        actuatorInfo = qMakePair(nodeName, actuatorType);
        groupMap[groupName].actuatorsInGroup.insert(actuatorName, actuatorInfo);
    } else                                                                                  //Gives qDebug that actuator has already been added
    {
        qDebug() << "This actuator has already been added";
    }
}

//Returns all groups
QStringList Groups::getGroups()
{
    // Get all keys from the QMap
    QStringList groups = groupMap.keys();
    //returns all group names
    return groups;
}

//Returns all sensors from a group
QStringList Groups::getSensors(QString groupName)
{
    QStringList sensorInfoReturn;
    for (const QString& sensorNameReturn : groupMap[groupName].sensorsInGroup.uniqueKeys())
    {
        for (const QPair<QString, QString>& sensorInfo : groupMap[groupName].sensorsInGroup.values(sensorNameReturn))
        {
            QString returnSensorString = sensorNameReturn + '\t' + sensorInfo.first + '\t' + sensorInfo.second;
            sensorInfoReturn.append(returnSensorString);
        }
    }
    return sensorInfoReturn;
}

//Returns all Actuators from a group
QStringList Groups::getActuators(QString groupName)
{
    QStringList actuatorInfoReturn;
    for (const QString& actuatorNameReturn : groupMap[groupName].actuatorsInGroup.uniqueKeys())
    {
        for (const QPair<QString, QString>& actuatorInfo : groupMap[groupName].actuatorsInGroup.values(actuatorNameReturn))
        {
            QString returnSensorString = actuatorNameReturn + '\t' + actuatorInfo.first + '\t' + actuatorInfo.second;
            actuatorInfoReturn.append(returnSensorString);
        }
    }
    return actuatorInfoReturn;
}

//Deletes a group
void Groups::deleteGroupInstance(QString groupName)
{
    groupMap.remove(groupName);
    groupAddress.remove(groupName);
    auto it = sensorGroupLink.begin();
    while (it != sensorGroupLink.end()) {
        if (it.value() == groupName) {
            it = sensorGroupLink.erase(it);
        } else {
            ++it;
        }
    }
    qDebug() << "Group: " << groupName << " has been deleted";
}

//Deletes a sensor
void Groups::deleteSensor(QString groupName, QString sensorName)
{
    groupMap[groupName].sensorsInGroup.remove(sensorName);
}

//Deletes a actuator
void Groups::deleteActuator(QString groupName, QString actuatorName)
{
    groupMap[groupName].actuatorsInGroup.remove(actuatorName);
}

//checks if the sensor is linked to any groups
QStringList Groups::checkGroups(QString sensorName, QString nodeName)
{
    sensorInfo = qMakePair(sensorName, nodeName);
    QStringList groupList;
    foreach (const QString& group, sensorGroupLink.values(sensorInfo)) {
        groupList.append(group);
    }
    return groupList;
}

int Groups::getGroupAddress(QString groupName)
{
    return groupAddress.value(groupName);
}
