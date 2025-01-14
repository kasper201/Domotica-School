#ifndef EASYSTRING_H
#define EASYSTRING_H

#include <QString>

class EasyString
{
public:
    EasyString() = default;

    //Out
    QString connectedOut = "mesh init";
    QString meshCreate = "mesh cdb create";
    QString meshProvCreate = "mesh prov local 0 0x0001";
    QString meshReset = "mesh reset-local";

    //In
    QString connectedIn = "Connected";
};

#endif // EASYSTRING_H
