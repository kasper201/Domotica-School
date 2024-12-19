#ifndef GROUPS_H
#define GROUPS_H

#include "domotica.h"
#include <QObject>
#include <QLineEdit>
#include <QMap>


class Groups : public QObject
{
    Q_OBJECT

public:
    explicit Groups(Domotica* domotica, QObject* parent = nullptr);
    ~Groups();
    void AddGroup(QString groupName);
    void DeleteGroup(QString groupName);

public slots:
    void handleGroupAdd();
    void handleGroupDelete();

private:
    Domotica* UIdomotica; // Pointer to the Domotica instance
    struct groupParts {
        QMultiHash<QString, QPair<QString, QString>> sensorsInGroup;
        QMultiHash<QString, QPair<QString, QString>> actuatorsInGroup;
    };

    QMap<QString, groupParts> groupsMap;
};

#endif // GROUPS_H
