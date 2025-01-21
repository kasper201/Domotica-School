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
    QString meshAppKey = " 0x000";
    QString meshServer = " 0x1000";
    QString meshClient = " 0x1001";
    QString meshSubscribe = "mesh models cfg model sub-add";
    QString meshUnsubscribe = "mesh models cfg model sub-del";

    //easy use
    QString hexPrefix = " 0x";

    //In
    QString connectedIn = "Connected";
};

#endif // EASYSTRING_H
