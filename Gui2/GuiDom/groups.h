#ifndef GROUPS_H
#define GROUPS_H

#include "domotica.h"
#include "nodes.h"
#include <QObject>
#include <QLineEdit>
#include <QMap>


class Groups : public QObject
{
    Q_OBJECT

public:
    explicit Groups(Domotica* domotica, Nodes* nodes, QObject* parent = nullptr);
    ~Groups();
    void AddGroup(QString groupName);
    void DeleteGroup(QString groupName);

public slots:
    void handleGroupAdd();
    void handleGroupDelete();

private:
    Domotica* UIdomotica; // Pointer to the Domotica instance
    struct groupParts {
        QPair<QString, QString> sensorList;     //Nodename, Sensorname
        QPair<QString, QString> actuatorList;   //Nodename, Actuatorname
    };

    QMap<QString, groupParts> groupsMap;
};

#endif // GROUPS_H
