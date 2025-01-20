#ifndef COMPORT_H
#define COMPORT_H

#include "domotica.h"
#include "easystring.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QObject>

class Comport : public QObject
{
    Q_OBJECT

public:
    explicit Comport(Domotica* domotica, QObject* parent = nullptr);
    ~Comport();
    QSerialPort* COMPORT;
    void AddItem(QString itemText);
    void setupComportList();
    void setupComport(const QString &comPortName);
    void setupProv();

public slots:
    void handleComportConnection();     // Handle Connect
    void handleComportRefresh();        // Handle Refresh
    void ReadData();
    void WriteToComport(QString sendString);

private:
    Domotica* UIdomotica; // Pointer to the Domotica instance
    EasyString EasyString;

    bool connected = false;
    QString Current_Comport;
    QString Data_From_SerialPort;
    bool Is_Data_Recieved = false;
};

#endif // COMPORT_H
