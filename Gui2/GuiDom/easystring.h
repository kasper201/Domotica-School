#ifndef EASYSTRING_H
#define EASYSTRING_H

#include <QString>

class EasyString
{
public:
    EasyString() = default;

    //Out
    QString connectedOut = "mesh init\r";

    //In
    QString connectedIn = "Connected";
};

#endif // EASYSTRING_H
