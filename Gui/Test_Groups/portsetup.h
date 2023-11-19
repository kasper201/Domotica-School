#ifndef PORTSETUP_H
#define PORTSETUP_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QLabel>

class PortSetup
{
public:
    PortSetup();
    ~PortSetup();
    QSerialPort* COMPORT;

    void setupComport(const QString &comPortName);
    void WriteToComport(QString sendString);

private:
    //COMPORT!!
    QString Current_Comport;


};

#endif // PORTSETUP_H
