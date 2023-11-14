#ifndef PORTSETUP_H
#define PORTSETUP_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QLabel>

#include "mainwindow.h"

class PortSetup
{
public:
    PortSetup();
    QSerialPort* COMPORT;

private:
    //COMPORT!!
    QString Current_Comport;


};

#endif // PORTSETUP_H
