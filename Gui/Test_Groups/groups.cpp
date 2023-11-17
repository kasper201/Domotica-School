#include "groups.h"

Groups::Groups()
{

}

Groups::~Groups()
{

}

void Groups::addGroupInstance(QString groupName)
{
    groupParts newGroupParts;
    groupMap.insert(groupName, newGroupParts);
}

void Groups::deleteGroupInstance(QString groupName)
{
    groupMap.remove(groupName);
    qDebug() << "Group: " << groupName << " has been deleted";
}

void Groups::addSensor(QString groupName, QString nodeName, QString sensorType, QString sensorName)
{
    bool alreadyExist = false;
    sensorInfo = qMakePair(sensorName, nodeName);
    for (const QString& sensorNameError : groupMap[groupName].sensorsInGroup.uniqueKeys()) //checks if the sensor is already added to the group
    {
        if(sensorName == sensorNameError)
        {
            alreadyExist = true;
        }
    }
    if(alreadyExist == false)
    {
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
    } else
    {
        qDebug() << "This sensor has already been added";
    }
}

QStringList Groups::getGroups()
{
    // Get all keys from the QMap
    QStringList groups = groupMap.keys();
    //returns all group names
    return groups;
}
