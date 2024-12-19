#include "comport.h"

#include <QListWidgetItem>

Comport::Comport(Domotica* domotica,  QObject* parent) : QObject(parent), UIdomotica(domotica)
{
}

Comport::~Comport(){}

//Add an item to the QListWidget
void Comport::AddItem(QString itemText)
{
    if (UIdomotica && UIdomotica->GetComportList()) {
        UIdomotica->GetComportList()->addItem(itemText); // Add the item
        qDebug() << "Item added: " << itemText;
    }
    else
    {
        qDebug() << "nullptr";
    }
}

//Show all current devices connected
void Comport::setupComportList()
{
    UIdomotica->GetComportList()->clear();
    //Puts all comports in a list
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        QString portName = port.portName();
        portName = portName.leftJustified(8, ' ');
        QString portDescription = port.description();
        QString portCombined = portName + "| " + portDescription;
        AddItem(portCombined);
    }
}

//Handles the button to connect/disconnect
void Comport::handleComportConnection()
{
    if(connected == false && UIdomotica->GetComportList()->currentItem()) //Connect to a new Comport
    {
        QString comString = UIdomotica->GetComportList()->currentItem()->text();
        QString firstSixCharacters = comString.left(4);
        QString comPortName =firstSixCharacters.replace(" ", "");
        qDebug() << comPortName;
        setupComport(comPortName);
        UIdomotica->GetComportLabel()->setText(comPortName);
        UIdomotica->GetConnectButton()->setText("Disconnect");
        connected = true;
    }
    else                                                                 //Disconnect
    {
        UIdomotica->GetComportLabel()->setText("No connection");
        UIdomotica->GetConnectButton()->setText("Connect");
        connected = false;
    }
}

//Handles the refresh of the comport list
void Comport::handleComportRefresh()
{
    setupComportList();
    qDebug() << "Comport list has been refreshed";
}

//Connect to the comport
void Comport::setupComport(const QString &comPortName)
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
        WriteToComport("connected");
    }
    else {
        qDebug() << "Serial Port is not connected";
        //qDebug() << COMPORT->error();
    }
}

//Send a message
void Comport::WriteToComport(QString sendString)
{
    //qDebug() << "Data send: " << sendString;
    COMPORT->write(sendString.toLatin1() + char(10) );
}
