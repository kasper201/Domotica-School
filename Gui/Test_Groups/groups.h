#ifndef GROUPS_H
#define GROUPS_H

#include <QDebug>
#include <QString>
#include <QMultiHash>
#include <QMap>
#include <QPair>
#include <map>

class Groups
{
public:
    Groups();
    ~Groups();
    void addGroupInstance(QString groupName);                                                           //Adds a new group
    void deleteGroupInstance(QString groupName);                                                        //Deletes a group
    void addSensor(QString groupName, QString nodeName, QString sensorType, QString sensorName);        //Adds a sensor to a group
    void addActuator(QString groupName, QString nodeName, QString actuatorType, QString actuatorName);  //Adds a actuator to a group

    QStringList getGroups();

    struct groupParts {
        QMultiHash<QString, QPair<QString, QString>> sensorsInGroup;    //Sensor name, Node name, Sensor type
        QMultiHash<QString, QPair<QString, QString>> actuatorsInGroup;  //Actuator name, Node name, Actuator type
    };


private:
    QMap<QString, groupParts> groupMap;

    QPair<QString, QString> sensorInfo;                             //sensorName, nodeName
    QMultiHash<QPair<QString, QString>, QString> sensorGroupLink;   //Stores the following: Sensor name, Node name, Group name

    QPair<QString, QString> actuatorInfo;                           //actuatorName, nodeName
    QMultiHash<QPair<QString, QString>, QString> actuatorGroupLink; //Stores the following: Actuator name, Node name, Group name
};

#endif // GROUPS_H
