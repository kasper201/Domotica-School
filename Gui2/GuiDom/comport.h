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
    QSerialPort* COMPORT;
    void AddItem(QString itemText);
    void setupComportList();
    void setupComport(const QString &comPortName);
    void setupProv();
    void AddNewNode(QString uuid);

signals:
    void updateGroup(int groupAddress, bool newState);

public slots:
    void handleComportConnection();     // Handle Connect
    void handleComportRefresh();        // Handle Refresh
    void ReadData();
    void SubcribeToGroup(QString groupName, QString nodeName, bool server);
    void UnsubcribeFromGroup(QString groupName, QString nodeName, bool server);
    void SendOutComputerSensor(int groupAddress, bool isGroupOn);
    void WriteToComport(QString sendString);

private:
    Domotica* UIdomotica; // Pointer to the Domotica instance
    Nodes* nodes;
    Groups* groups;
    InputChecks inputChecks;

    bool connected = false;
    QString Current_Comport;
    QString Data_From_SerialPort;
    bool Is_Data_Recieved = false;
    QStringList uuidList;
};

#endif // COMPORT_H
