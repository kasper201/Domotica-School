#ifndef COMPORT_H
#define COMPORT_H

#include "domotica.h"
#include "nodes.h"
#include "groups.h"
#include "inputchecks.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QObject>

class Comport : public QObject
{
    Q_OBJECT

public:
    explicit Comport(Domotica* domotica, Nodes* nodes, Groups* groups, QObject* parent = nullptr);
    ~Comport();
    void AddItem(QString itemText);
    void setupComportList();
    void setupComport(const QString &comPortName);
    void setupProv();
    void AddNewNode(QString uuid);

public slots:
    void handleComportConnection();     // Handle Connect
    void handleComportRefresh();        // Handle Refresh
    void ReadData();
    void SubcribeToGroup(QString groupName, QString nodeName, bool server);
    void UnsubcribeFromGroup(QString groupName, QString nodeName, bool server);
    void SendOutComputerStatusRequest();
    void SendOutForceState(bool turnOn);
    void removeNode();
    void WriteToComport(QString sendString);

private:
    QSerialPort* COMPORT = nullptr;
    Domotica* UIdomotica; // Pointer to the Domotica instance
    Nodes* nodes;
    Groups* groups;
    InputChecks inputChecks;

    bool connected = false;
    QString Current_Comport;
    QString Data_From_SerialPort;
    bool Is_Data_Recieved = false;
    bool getState = false;
    QStringList uuidList;
};

#endif // COMPORT_H
