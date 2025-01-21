#ifndef GROUPS_H
#define GROUPS_H

#include "domotica.h"
#include "nodes.h"
#include <QObject>
#include <QLineEdit>
#include <QMap>
#include <QMultiHash>


class Groups : public QObject
{
    Q_OBJECT

public:
    explicit Groups(Domotica* domotica, Nodes* nodes, QObject* parent = nullptr);
    ~Groups();
    void AddGroup(QString groupName);
    void DeleteGroup(QString groupName);
    void AddSensor(QString groupName, QString nodeName, QString sensorName);
    void AddActuator(QString groupName, QString nodeName, QString actuatorName);
    int GetGroupAddress(QString groupName);

signals:
    void subscribeToGroup(QString groupName, QString nodeName, bool server);

public slots:
    void handleGroupAdd();
    void handleGroupDelete();
    void handleSensorAdd();
    void handleActuatorAdd();
    void showGroup(QString groupName);

private:
    Domotica* UIdomotica; // Pointer to the Domotica instance
    struct groupParts {
        int groupAddress;                            //Address for the mesh network
        QMultiHash<QString, QString> sensorList;     //Nodename, Sensorname
        QMultiHash<QString, QString> actuatorList;   //Nodename, Actuatorname
    };

    QMap<QString, groupParts> groupsMap;

    int FirstFreeGroupAddress();
};

#endif // GROUPS_H
