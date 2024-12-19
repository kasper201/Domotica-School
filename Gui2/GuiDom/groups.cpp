#include "groups.h"

Groups::Groups(Domotica* domotica,  QObject* parent) : QObject(parent), UIdomotica(domotica)
{
}

Groups::~Groups(){}

void Groups::handleGroupAdd()
{
    qDebug() << "Add";
    QString groupName = UIdomotica->GetGroupName()->text();
    groupParts newGroupParts;
    groupsMap.insert(groupName, newGroupParts);
    UIdomotica->GetGroupList()->addItem(groupName);
}

void Groups::handleGroupDelete()
{
    qDebug() << "Delete";
}
