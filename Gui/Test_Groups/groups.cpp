#include "groups.h"

Groups::Groups()
{

}

Groups::~Groups()
{

}

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
        qDebug() << "Group: " << groupName << " has been added";
    } else
    {
        qDebug() << "Group: " << groupName << " already exists";
    }
}

void Groups::deleteGroupInstance(QString groupName)
{
    groupMap.remove(groupName);
    qDebug() << "Group: " << groupName << " has been deleted";
}

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

        for (const QString& sensorNameD : groupMap[groupName].sensorsInGroup.uniqueKeys())
        {
            qDebug() << "Sensor Name:" << sensorNameD;

            for (const QPair<QString, QString>& sensorInfo : groupMap[groupName].sensorsInGroup.values(sensorNameD))
            {
                qDebug() << "Node Name:" << sensorInfo.first << "Sensor Type:" << sensorInfo.second;
            }
        }
    } else                                                                                  //Gives qDebug that sensor has already been added
    {
        qDebug() << "This sensor has already been added";
    }
}

void Groups::addActuator(QString groupName, QString nodeName, QString actuatorType, QString actuatorName)
{
    bool alreadyExist = false;
    for (const QString& sensorNameError : groupMap[groupName].sensorsInGroup.uniqueKeys()) //checks if the actuator is already added to the group
    {
        if(actuatorName == sensorNameError)
        {
            alreadyExist = true;
        }
    }
    if(alreadyExist == false)                                                               //Adds actuator if it is not present yet
    {
        sensorInfo = qMakePair(actuatorName, nodeName);
        sensorGroupLink.insert(sensorInfo, groupName);
        sensorInfo = qMakePair(nodeName, actuatorType);
        groupMap[groupName].sensorsInGroup.insert(actuatorName, sensorInfo);

        for (const QString& sensorNameD : groupMap[groupName].sensorsInGroup.uniqueKeys())
        {
            qDebug() << "Sensor Name:" << sensorNameD;

            for (const QPair<QString, QString>& sensorInfo : groupMap[groupName].sensorsInGroup.values(sensorNameD))
            {
                qDebug() << "Node Name:" << sensorInfo.first << "Sensor Type:" << sensorInfo.second;
            }
        }
    } else                                                                                  //Gives qDebug that actuator has already been added
    {
        qDebug() << "This actuator has already been added";
    }
}

QStringList Groups::getGroups()
{
    // Get all keys from the QMap
    QStringList groups = groupMap.keys();
    //returns all group names
    return groups;
}
