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
    Comport* comport = new Comport(domotica);
    Nodes* nodes = new Nodes(domotica);
    Groups* groups = new Groups(domotica, nodes);

    // Connect all Signals
    QObject::connect(domotica, &Domotica::comportConnectionRequested,
                     comport, &Comport::handleComportConnection);
    QObject::connect(domotica, &Domotica::comportRefreshRequested,
                     comport, &Comport::handleComportRefresh);
    QObject::connect(domotica, &Domotica::groupAddRequested,
                     groups, &Groups::handleGroupAdd);
    QObject::connect(domotica, &Domotica::groupDeleteRequested,
                     groups, &Groups::handleGroupDelete);
    QObject::connect(domotica, &Domotica::updateNodeParts,
                     nodes, &Nodes::showParts);

    //Add Computer node
    nodes->addNode("Computer");
    nodes->addSensorToNode("Computer", "ApplicationButton");
    nodes->addActuatorToNode("Computer", "ApplicationLED");

    comport->setupComportList();
    domotica->show();
    return a.exec();
}
