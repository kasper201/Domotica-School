#include "comport.h"

#include <QListWidgetItem>
#include <QTimer>

Comport::Comport(Domotica* domotica , Nodes* nodes, Groups* groups,  QObject* parent) : QObject(parent), UIdomotica(domotica), nodes(nodes), groups(groups)
{
}

Comport::~Comport()
{
    WriteToComport(EasyString.meshReset);
    COMPORT->close();
}

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
        WriteToComport(EasyString.meshReset);
        QTimer::singleShot(100, [this](){ COMPORT->close(); });
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
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud115200);
    COMPORT->setParity(QSerialPort::Parity::NoParity);
    COMPORT->setDataBits(QSerialPort::DataBits::Data8);
    COMPORT->setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    COMPORT->open(QIODevice::ReadWrite);

    if(COMPORT->isOpen()) {
        connect(COMPORT, SIGNAL(readyRead()), this, SLOT(ReadData()));
        setupProv(); //Trigger setup of provisioner
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
    qDebug() << "Data send: " << sendString;
    QString close = "\r\n";
    COMPORT->write(sendString.toLatin1() + close.toLatin1() );
}

//setup the provisioner and appkey
void Comport::setupProv()
{
    WriteToComport(EasyString.connectedOut);
    QTimer::singleShot(15, this, [this](){
        qDebug() << "Timer has passed";
        WriteToComport(EasyString.meshCreate);
    });
    QTimer::singleShot(80, this, [this](){
        qDebug() << "Timer has passed";
        WriteToComport(EasyString.meshProvCreate);
    });
    QTimer::singleShot(150, this, [this](){
        qDebug() << "Timer has passed";
        WriteToComport(EasyString.meshAppKeyCreate);
    });
    QTimer::singleShot(300, this, [this](){
        qDebug() << "Timer has passed";
        WriteToComport(EasyString.meshAppKeyBind + EasyString.meshAppKeyProv + EasyString.meshAppKey + EasyString.meshServer);
    });
    QTimer::singleShot(350, this, [this](){
        qDebug() << "Timer has passed";
        WriteToComport(EasyString.meshAppKeyBind + EasyString.meshAppKeyProv + EasyString.meshAppKey + EasyString.meshClient);
    });
    nodes->addNode("Provisioner", 1);
    nodes->addActuatorToNode("Provisioner", "Provisioner Light");
    nodes->addSensorToNode("Provisioner", "Provisioner Sensor");
}

//Subscibes the model to a group
void Comport::SubcribeToGroup(QString groupName, QString nodeName, bool server)
{
    //Use the addresses and send them in the right way ( adds 0x as prefix from easystring and than a 4 didget hexadecimal number)
    QString nodeAddress = EasyString.hexPrefix + QString("%1").arg(QString::number(nodes->getNodeAddress(nodeName), 16).rightJustified(4, '0'));
    QString groupAddress = EasyString.hexPrefix + QString("%1").arg(QString::number(groups->GetGroupAddress(groupName), 16).rightJustified(4, '0'));
    if(server){
        WriteToComport(EasyString.meshSubscribe + nodeAddress + groupAddress + EasyString.meshServer);
    } else {
        WriteToComport(EasyString.meshSubscribe + nodeAddress + groupAddress + EasyString.meshClient);
    }
}

//Unsubscribes the model from a group
void Comport::UnsubcribeFromGroup(QString groupName, QString nodeName, bool server)
{
    //Use the addresses and send them in the right way ( adds 0x as prefix from easystring and than a 4 didget hexadecimal number)
    QString nodeAddress = EasyString.hexPrefix + QString("%1").arg(QString::number(nodes->getNodeAddress(nodeName), 16).rightJustified(4, '0'));
    QString groupAddress = EasyString.hexPrefix + QString("%1").arg(QString::number(groups->GetGroupAddress(groupName), 16).rightJustified(4, '0'));
    if(server){
        WriteToComport(EasyString.meshUnsubscribe + nodeAddress + groupAddress + EasyString.meshServer);
    } else {
        WriteToComport(EasyString.meshUnsubscribe + nodeAddress + groupAddress + EasyString.meshClient);
    }
}

//Reads incoming data
void Comport::ReadData()
{
    if(COMPORT->isOpen())
    {
        //Reads uart
        while(COMPORT->bytesAvailable())
        {
            Data_From_SerialPort += COMPORT->readAll();

            //Checks if the line has ended
            if(Data_From_SerialPort.contains("$"))
            {
                Is_Data_Recieved = true;
            }
        }

        //turns data recieved off again
        if(Is_Data_Recieved == true)
        {
            Data_From_SerialPort.remove("\r").remove("\n");
            Data_From_SerialPort.remove("\u001B[1;32muart:~$").remove("\u001B[m").remove("\u001B[8D").remove("\u001B[0m").remove(Data_From_SerialPort.indexOf("\u001B[J"), 22);
            Data_From_SerialPort.remove("\u001B{");
            Data_From_SerialPort.remove(EasyString.connectedOut).remove(EasyString.meshCreate).remove(EasyString.meshProvCreate);
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
