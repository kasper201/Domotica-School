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
        COMPORT->close();
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
        connect(COMPORT, SIGNAL(readyRead()), this, SLOT(ReadData()));
        WriteToComport(EasyString.connectedOut);
        qDebug() << "Serial Port is connected";
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

void Comport::ReadData()
{
    if(COMPORT->isOpen())
    {
        //Reads uart
        while(COMPORT->bytesAvailable())
        {
            Data_From_SerialPort += COMPORT->readAll();

            //Checks if the line has ended
            if(Data_From_SerialPort.at(Data_From_SerialPort.length() - 1) == char(10))
            {
                Is_Data_Recieved = true;
            }
        }

        //turns data recieved off again
        if(Is_Data_Recieved == true)
        {
            Data_From_SerialPort.remove("\r").remove("\n");
            Is_Data_Recieved = false;
            qDebug() << "Message recieved: " << Data_From_SerialPort;

            if(Data_From_SerialPort.contains(EasyString.connectedIn))
            {
                qDebug() << "External device is connected";
            }

            //Reset Data recieved
            Data_From_SerialPort = "";
        }

    }
    else
    {
        qDebug() << COMPORT->error();
    }
}
