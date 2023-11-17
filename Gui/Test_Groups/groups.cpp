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
}

void Groups::addSensor(QString groupName, QString nodeName, QString sensorType, QString sensorName)
{
    sensorInfo = qMakePair(nodeName, sensorType);
    groupMap[groupName].sensorsInGroup.insert(sensorName, sensorInfo);
    sensorInfo = qMakePair(sensorName, nodeName);
    sensorGroupLink.insert(sensorInfo, groupName);

    for (const QString& group : groupMap[groupName].sensorsInGroup.uniqueKeys())
    {
        qDebug() << "Group:" << group;

        for (const QPair<QString, QString>& sensorInfo : groupMap[groupName].sensorsInGroup.values(group))
        {
            qDebug() << "  Node Name:" << sensorInfo.first << "Sensor Type:" << sensorInfo.second;
        }
    }
}

QStringList Groups::getGroups()
{
    // Get all keys from the QMap
    QStringList groups = groupMap.keys();
    //returns all group names
    return groups;
}
