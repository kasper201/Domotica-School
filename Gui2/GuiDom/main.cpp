#include "domotica.h"
#include "comport.h"
#include "groups.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Domotica* domotica = new Domotica;
    Comport* comport = new Comport(domotica);
    Groups* groups = new Groups(domotica);

    // Connect all Signals
    QObject::connect(domotica, &Domotica::comportConnectionRequested,
                     comport, &Comport::handleComportConnection);
    QObject::connect(domotica, &Domotica::comportRefreshRequested,
                     comport, &Comport::handleComportRefresh);
    QObject::connect(domotica, &Domotica::groupAddRequested,
                     groups, &Groups::handleGroupAdd);
    QObject::connect(domotica, &Domotica::groupDeleteRequested,
                     groups, &Groups::handleGroupDelete);

    comport->setupComportList();
    domotica->show();
    return a.exec();
}
