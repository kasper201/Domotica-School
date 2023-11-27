#include "movedfunctions.h"

MovedFunctions::MovedFunctions()
{

}

MovedFunctions::~MovedFunctions()
{

}

//Adds new nodes
QString MovedFunctions::addNodes(QString input, Node& node, QListWidget* nodeList)
{
    if(input.contains("AddNode")) //Start the process of adding a new node
    {
        //Removes parts of the string that are irrelevant
        input = stringM.removedTillWhitespace(input);

        //Adds a node
        QString nodeName = stringM.removedFromWhitespace(input);
        node.addNodeInstance(nodeName);
        nodeList->addItem(nodeName);
        input = stringM.removedTillWhitespace(input);

        //Adds all sensors
        while(input.contains("AddSensor"))
        {
            input = stringM.removedTillWhitespace(input);
            QString sensorType = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            QString sensorName = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            node.addSensor(nodeName, sensorType, sensorName);
        }

        //Adds all actuators
        while(input.contains("AddActuator"))
        {
            input = stringM.removedTillWhitespace(input);
            QString actuatorType = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            QString actuatorName = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            QString actuatorStatus = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            node.addActuator(nodeName, actuatorType, actuatorName, actuatorStatus);
        }

        input = "";
    }

    return input;
}

//Adds new group
QString MovedFunctions::addGroups(QString input, Groups& groups, QListWidget* groupList)
{
    if(input.contains("AddGroup"))
    {
        input = stringM.removedTillWhitespace(input);

        //Adds a group
        QString groupName = stringM.removedFromWhitespace(input);
        groups.addGroupInstance(groupName);
        groupList->addItem(groupName);
        input = stringM.removedTillWhitespace(input);

        while(input.contains("AddSensor"))
        {
            input = stringM.removedTillWhitespace(input);
            QString nodeName = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            QString sensorType = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            QString sensorName = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            groups.addSensor(groupName, nodeName, sensorType, sensorName);
        }

        while(input.contains("AddActuator"))
        {
            input = stringM.removedTillWhitespace(input);
            QString nodeName = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            QString actuatorType = stringM.removedFromWhitespace(input);
            qDebug() << actuatorType;
            input = stringM.removedTillWhitespace(input);
            QString actuatorName = stringM.removedFromWhitespace(input);
            input = stringM.removedTillWhitespace(input);
            groups.addActuator(groupName, nodeName, actuatorType, actuatorName);
        }

        input = "";
    }
    return input;
}

//Updates overview of sensors and actuators of current group
void MovedFunctions::updateCurrentGroupOverview(QListWidget* sensorListGroup, QListWidget* actuatorListGroup, QListWidget* groupList, Groups& groups)
{
    sensorListGroup->clear();
    actuatorListGroup->clear();

    QListWidgetItem* headerItem = new QListWidgetItem("Sensor\tNode\tType");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    sensorListGroup->addItem(headerItem);

    headerItem = new QListWidgetItem("Actuator\tNode\tType");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    actuatorListGroup->addItem(headerItem);

    if(!groupList->selectedItems().isEmpty())
    {
        QString groupName = groupList->currentItem()->text();

        QStringList sensorsInGroup = groups.getSensors(groupName);
        if(!sensorsInGroup.isEmpty())
        {
            foreach (const QString &sensorInformation, sensorsInGroup)   //Adds all groups to groupLinkList
            {
                sensorListGroup->addItem(sensorInformation);
            }
        }

        QStringList actuatorsInGroup = groups.getActuators(groupName);
        if(!actuatorsInGroup.isEmpty())
        {
            foreach (const QString &actuatorInformation, actuatorsInGroup)   //Adds all groups to groupLinkList
            {
                actuatorListGroup->addItem(actuatorInformation);
            }
        }
    }
}


//adds titles to all listWidgets on node page
void MovedFunctions::addTitles(bool nodeNotNeeded, QListWidget* nodeList, QListWidget* sensorList, QListWidget* actuatorList)
{
    QListWidgetItem *headerItem = new QListWidgetItem("Nodes");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    if(nodeNotNeeded == false)
    {
        nodeList->addItem(headerItem);
    }
    headerItem = new QListWidgetItem("Sensor\tType");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    sensorList->addItem(headerItem);
    headerItem = new QListWidgetItem("Actuator\tType\tStatus");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    actuatorList->addItem(headerItem);
}

//Updates all lists that show groups when needed
void MovedFunctions::updateGroupLists(QListWidget* groupList, QListWidget* groupLinkList, QPushButton* sensorAddButton, QPushButton* actuatorAddButton, Groups& groups, QListWidget* nodeList)
{
    groupList->clear();
    groupLinkList->clear();
    QStringList allGroups = groups.getGroups();

    QListWidgetItem* headerItem = new QListWidgetItem("Groups");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    groupList->addItem(headerItem);
    headerItem = new QListWidgetItem("Groups");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    groupLinkList->addItem(headerItem);

    foreach (const QString &groupName, allGroups)   //Adds all groups to groupList
    {
        groupList->addItem(groupName);
    }

    foreach (const QString &groupName, allGroups)   //Adds all groups to groupLinkList
    {
        groupLinkList->addItem(groupName);
    }

    if(groupLinkList->count() > 1 && !nodeList->selectedItems().isEmpty())
    {
        groupLinkList->show();
        sensorAddButton->show();
        actuatorAddButton->show();
    } else
    {
        groupLinkList->hide();
        sensorAddButton->hide();
        actuatorAddButton->hide();
    }
}
