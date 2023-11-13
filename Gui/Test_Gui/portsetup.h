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
    QSerialPort* COMPORT;

private:
    //COMPORT!!
    QString Current_Comport = "COM3";


};

#endif // PORTSETUP_H
