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
    void addGroupInstance(QString groupName);
    void deleteGroupInstance(QString groupName);
    void addSensor(QString groupName, QString nodeName, QString sensorType, QString sensorName);

    QStringList getGroups();

    struct groupParts {
        QMultiHash<QString, QPair<QString, QString>> sensorsInGroup; //Sensor name, Node name, Sensor type
        QMultiHash<QString, QString> actuatorsInGroup;
    };


private:
    QMap<QString, groupParts> groupMap;

    QPair<QString, QString> sensorInfo;
    QMultiHash<QPair<QString, QString>, QString> sensorGroupLink; //Sensor name, Node name, Group name
};

#endif // GROUPS_H
