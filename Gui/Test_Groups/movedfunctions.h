#ifndef MOVEDFUNCTIONS_H
#define MOVEDFUNCTIONS_H

#include <QListWidget>
#include <QPushButton>

#include "stringmodifiers.h"
#include "node.h"
#include "groups.h"

class MovedFunctions
{
public:
    MovedFunctions();
    ~MovedFunctions();

    QString addNodes(QString input, Node& node, StringModifiers& stringM, QListWidget* nodeList);
    void updateCurrentGroupOverview(QListWidget* sensorListGroup, QListWidget* actuatorListGroup, QListWidget* groupList, Groups& groups);
    void addTitles(bool nodeNotNeeded, QListWidget* nodeList, QListWidget* sensorList, QListWidget* actuatorList);
    void updateGroupLists(QListWidget* groupList, QListWidget* groupLinkList, QPushButton* sensorAddButton, QPushButton* actuatorAddButton, Groups& groups, QListWidget* nodeList);

private:
};

#endif // MOVEDFUNCTIONS_H
