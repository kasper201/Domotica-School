#include "portsetup.h"

PortSetup::PortSetup()
{

}

PortSetup::~PortSetup()
{
    //COMPORT is closed in ~MainWindow
}

void PortSetup::setupComport(const QString &comPortName)
{
    Current_Comport = comPortName;
    COMPORT = new QSerialPort();
    COMPORT->setPortName(Current_Comport);
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud115200);
    COMPORT->setParity(QSerialPort::Parity::NoParity);
    COMPORT->setDataBits(QSerialPort::DataBits::Data8);
    COMPORT->setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    COMPORT->open(QIODevice::ReadWrite);

    if(COMPORT->isOpen()) {
        //qDebug() << "Serial Port is connected";
    }
    else {
        //qDebug() << "Serial Port is not connected";
        //qDebug() << COMPORT->error();
    }
}

//send string to comport
void PortSetup::WriteToComport(QString sendString)
{
    //qDebug() << "Data send: " << sendString;
    COMPORT->write(sendString.toLatin1() + char(10) );
}
