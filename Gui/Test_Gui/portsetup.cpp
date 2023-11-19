#include "portsetup.h"

PortSetup::PortSetup(const QString &comPortName)
{
    Current_Comport = comPortName;
    COMPORT = new QSerialPort();
    COMPORT->setPortName(Current_Comport);
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud9600);
    COMPORT->setParity(QSerialPort::Parity::NoParity);
    COMPORT->setDataBits(QSerialPort::DataBits::Data8);
    COMPORT->setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    COMPORT->open(QIODevice::ReadWrite);

    if(COMPORT->isOpen()) {
        qDebug() << "Serial Port is connected";
    }
    else {
        qDebug() << "Serial Port is not connected";
        qDebug() << COMPORT->error();
    }
}

PortSetup::~PortSetup()
{
    //COMPORT is closed in ~MainWindow
}
