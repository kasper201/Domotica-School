#ifndef COMPORT_H
#define COMPORT_H

#include "domotica.h"
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
    void WriteToComport(QString sendString);

public slots:
    void handleComportConnection();     // Handle Connect
    void handleComportRefresh();        // Handle Refresh

private:
    Domotica* UIdomotica; // Pointer to the Domotica instance
    bool connected = false;
    QString Current_Comport;
};

#endif // COMPORT_H
