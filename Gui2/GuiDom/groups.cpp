#include "groups.h"

Groups::Groups(Domotica* domotica,  QObject* parent) : QObject(parent), UIdomotica(domotica)
{
}

Groups::~Groups(){}

//Add a new Group
void Groups::AddGroup(QString groupName)
{
    if(!groupName.isEmpty())
    {
        bool alreadyExist = false;
        for (const QString& groupNameError : groupsMap.keys()) //checks if the actuator is already added to the group
        {
            if(groupName == groupNameError)
            {
                alreadyExist = true;
            }
        }
        if(!alreadyExist)
        {
            qDebug() << "Add: " << groupName;
            groupParts newGroupParts;
            groupsMap.insert(groupName, newGroupParts);
            UIdomotica->GetGroupList()->addItem(groupName);
            UIdomotica->GetGroupName()->clear();
        }
        else
        {
            qDebug() << "Group already exists";
            UIdomotica->GetGroupName()->clear();
        }
    }
    else
    {
        qDebug() << "Nothing to add";
    }
}

//Delete a Group
void Groups::DeleteGroup(QString groupName)
{
    if(!UIdomotica->GetGroupList()->selectedItems().isEmpty())
    {
        UIdomotica->GetGroupList()->takeItem(UIdomotica->GetGroupList()->currentRow());
        groupsMap.remove(groupName);
        qDebug() << "Deleted: " << groupName;
    }
    else
    {
    qDebug() << "Select a Group to delete";
    }
}


void Groups::handleGroupAdd()
{
    AddGroup(UIdomotica->GetGroupName()->text());
}

void Groups::handleGroupDelete()
{
    DeleteGroup(UIdomotica->GetGroupList()->currentItem()->text());
}
