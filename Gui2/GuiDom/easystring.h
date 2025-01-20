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
    QString meshAppKeyCreate = "mesh models cfg appkey add 0 0";
    QString meshAppKeyBind = "mesh models cfg model app-bind ";
    QString meshAppKeyProv = "0x0001";
    QString meshAppKeyClient = " 0x000 0x1000";
    QString meshAppKeyServer = " 0x000 0x1001";

    //In
    QString connectedIn = "Connected";
};

#endif // EASYSTRING_H
