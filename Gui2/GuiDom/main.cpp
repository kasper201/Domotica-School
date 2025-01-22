#include "domotica.h"
#include "comport.h"
#include "groups.h"
#include "nodes.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Domotica* domotica = new Domotica;
    Nodes* nodes = new Nodes(domotica);
    Groups* groups = new Groups(domotica);
    Comport* comport = new Comport(domotica, nodes, groups);

    // Connect all Signals
    QObject::connect(domotica, &Domotica::comportConnectionRequested,
                     comport, &Comport::handleComportConnection);
    QObject::connect(domotica, &Domotica::comportRefreshRequested,
                     comport, &Comport::handleComportRefresh);
    QObject::connect(domotica, &Domotica::groupAddRequested,
                     groups, &Groups::handleGroupAdd);
    QObject::connect(domotica, &Domotica::groupDeleteRequested,
                     groups, &Groups::handleGroupDelete);
    QObject::connect(domotica, &Domotica::groupSensorAdd,
                     groups, &Groups::handleSensorAdd);
    QObject::connect(domotica, &Domotica::groupActuatorAdd,
                     groups, &Groups::handleActuatorAdd);
    QObject::connect(domotica, &Domotica::updateGroupParts,
                     groups, &Groups::showGroup);
    QObject::connect(domotica, &Domotica::updateNodeParts,
                     nodes, &Nodes::showParts);
    QObject::connect(groups, &Groups::subscribeToGroup,
                     comport, &Comport::SubcribeToGroup);
    QObject::connect(domotica, &Domotica::groupSensorDeleteRequested,
                     groups, &Groups::handleSensorDelete);
    QObject::connect(domotica, &Domotica::groupActuatorDeleteRequested,
                     groups, &Groups::handleActuatorDelete);
    QObject::connect(groups, &Groups::unsubscribeFromGroup,
                     comport, &Comport::UnsubcribeFromGroup);
    QObject::connect(domotica, &Domotica::sendOutComputer,
                     groups, &Groups::handleComputerToGroups);
    QObject::connect(groups, &Groups::sendToGroup,
                     comport, &Comport::SendOutComputerSensor);
    QObject::connect(comport, &Comport::updateGroup,
                     groups, &Groups::handleGroupsToComputer);

    //Add Computer node
    nodes->addNode(COMPUTER_NODE_NAME, 0);
    nodes->addSensorToNode(COMPUTER_NODE_NAME, STRING_TO_SHOW_BUTTON + QString(NODE_SHOW) + COMPUTER_NODE_NAME);
    nodes->addActuatorToNode(COMPUTER_NODE_NAME, STRING_TO_SHOW_LED + QString(NODE_SHOW) + COMPUTER_NODE_NAME);

    comport->setupComportList();
    domotica->show();
    return a.exec();
}
