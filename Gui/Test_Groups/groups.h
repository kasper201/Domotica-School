#ifndef GROUPS_H
#define GROUPS_H

#include <QDebug>
#include <QString>
#include <QMultiHash>
#include <map>

class Groups
{
public:
    Groups();
    ~Groups();

    struct groupParts {
        QMultiHash<QString, QString> sensorsInGroup;
        QMultiHash<QString, QString> actuatorsInGroup;
    };

private:
    QMap<QString, groupParts> groupMap;
};

#endif // GROUPS_H
