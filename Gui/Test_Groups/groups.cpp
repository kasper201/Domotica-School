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
    qDebug() << groupName;
}
