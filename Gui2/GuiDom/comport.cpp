#include "comport.h"
#include "easystring.h"

#include <QListWidgetItem>
#include <QTimer>

Comport::Comport(Domotica* domotica , Nodes* nodes, Groups* groups,  QObject* parent) : QObject(parent), UIdomotica(domotica), nodes(nodes), groups(groups)
{
}

Comport::~Comport()
{
    WriteToComport(MESH_RESET);
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
        WriteToComport(MESH_RESET);
        QTimer::singleShot(100, [this](){ COMPORT->close(); });
        UIdomotica->GetComportLabel()->setText("No connection");
        UIdomotica->GetConnectButton()->setText("Connect");
        connected = false;
        uuidList.clear();
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
    if(COMPORT != nullptr) {
        qDebug() << "Data send: " << sendString;
        QString close = "\r\n";
        COMPORT->write(sendString.toLatin1() + close.toLatin1() );
    } else {
        qDebug() << "First make a connection";
    }
}

//setup the provisioner and appkey
void Comport::setupProv()
{
    WriteToComport(CONNECTED_OUT);
    QTimer::singleShot(15, this, [this](){
        qDebug() << "Timer has passed: mesh create";
        WriteToComport(MESH_CREATE);
    });
    QTimer::singleShot(80, this, [this](){
        qDebug() << "Timer has passed: provisioning";
        WriteToComport(MESH_PROV_CREATE);
    });
    QTimer::singleShot(250, this, [this](){
        qDebug() << "Timer has passed: appkey create";
        WriteToComport(MESH_APP_KEY_CREATE);
    });
    QTimer::singleShot(400, this, [this](){
        qDebug() << "Timer has passed: appkey bind 1";
        WriteToComport(MESH_APP_KEY_BIND + QString(MESH_ADDRESS_PROV) + MESH_APP_KEY + MESH_SERVER);
    });
    QTimer::singleShot(500, this, [this](){
        qDebug() << "Timer has passed: appkey bind 2";
        WriteToComport(MESH_APP_KEY_BIND + QString(MESH_ADDRESS_PROV) + MESH_APP_KEY + MESH_CLIENT);
    });
    // Easystring might make it unclear here but is is to easily change the names later on if needed
    nodes->addNode(PROVISIONER_NODE_NAME, 1);
    nodes->addActuatorToNode(PROVISIONER_NODE_NAME, QString(STRING_TO_SHOW_LED) + PROVISIONER_NODE_NAME);
    nodes->addSensorToNode(PROVISIONER_NODE_NAME, QString(STRING_TO_SHOW_BUTTON) + PROVISIONER_NODE_NAME);
    QTimer::singleShot(600, this, [this](){
        qDebug() << "Timer has passed: look for new nodes";
        WriteToComport(MESH_BEACON_ON);
    });
}

//Subscibes the model to a group
void Comport::SubcribeToGroup(QString groupName, QString nodeName, bool server)
{
    //Use the addresses and send them in the right way ( adds 0x as prefix from easystring and than a 4 didget hexadecimal number)
    QString nodeAddress = HEX_PREFIX + QString("%1").arg(QString::number(nodes->getNodeAddress(nodeName), 16).rightJustified(4, '0'));
    QString groupAddress = HEX_PREFIX + QString("%1").arg(QString::number(groups->GetGroupAddress(groupName), 16).rightJustified(4, '0'));
    WriteToComport(MESH_TARGET + nodeAddress);
    QTimer::singleShot(600, this, [this, server, nodeAddress, groupAddress](){
        if(server){
            WriteToComport(MESH_SUBSCRIBE + nodeAddress + groupAddress + MESH_SERVER);
        } else {
            WriteToComport(MESH_SUBSCRIBE + nodeAddress + groupAddress + MESH_CLIENT);
        }
    });
}

//Unsubscribes the model from a group
void Comport::UnsubcribeFromGroup(QString groupName, QString nodeName, bool server)
{
    //Use the addresses and send them in the right way ( adds 0x as prefix from easystring and than a 4 didget hexadecimal number)
    QString nodeAddress = HEX_PREFIX + QString("%1").arg(QString::number(nodes->getNodeAddress(nodeName), 16).rightJustified(4, '0'));
    QString groupAddress = HEX_PREFIX + QString("%1").arg(QString::number(groups->GetGroupAddress(groupName), 16).rightJustified(4, '0'));
    WriteToComport(MESH_TARGET + nodeAddress);
    QTimer::singleShot(600, this, [this, server, nodeAddress, groupAddress](){
        if(server){
            WriteToComport(MESH_UNSUBSCRIBE + nodeAddress + groupAddress + MESH_SERVER);
        } else {
            WriteToComport(MESH_UNSUBSCRIBE + nodeAddress + groupAddress + MESH_CLIENT);
        }
    });
}

void Comport::SendOutComputerStatusRequest()
{
    if (UIdomotica->GetNodeList()->currentRow() != -1 && COMPORT != nullptr) {
        QString nodeName = UIdomotica->GetNodeList()->currentItem()->text();
        int nodeAddressDec = nodes->getNodeAddress(nodeName);
        qDebug() << "Address: " << nodeAddressDec;
        QString nodeAddressHex = HEX_PREFIX + QString::number((nodeAddressDec), 16).rightJustified(4, '0');
        getState = true;
        WriteToComport(MESH_TARGET + nodeAddressHex);
        QTimer::singleShot(100, this, [this](){
            qDebug() << "Timer has passed: send request for status";
            WriteToComport(MESH_SEND + QString(MESH_GEN_STATUS));
        });
    } else if (COMPORT == nullptr) {
        qDebug() << "Connect to a device first";
    }
}

void Comport::SendOutForceState(bool turnOn)
{
    if (UIdomotica->GetNodeList()->currentRow() != -1 && COMPORT != nullptr) {
        QString nodeName = UIdomotica->GetNodeList()->currentItem()->text();
        int nodeAddressDec = nodes->getNodeAddress(nodeName);
        qDebug() << "Address: " << nodeAddressDec;
        QString nodeAddressHex = HEX_PREFIX + QString::number((nodeAddressDec), 16).rightJustified(4, '0');
        getState = true;
        WriteToComport(MESH_TARGET + nodeAddressHex);
        QTimer::singleShot(100, this, [this, turnOn](){
            qDebug() << "Timer has passed: send force status" << turnOn;
            if(turnOn){
                WriteToComport(MESH_SEND + QString(MESH_GEN_ON));
            } else {
                WriteToComport(MESH_SEND + QString(MESH_GEN_OFF));
            }
        });
    } else if (COMPORT == nullptr) {
        qDebug() << "Connect to a device first";
    }
}

void Comport::removeNode()
{
    if (UIdomotica->GetNodeList()->currentRow() != -1 && COMPORT != nullptr) {
        QString nodeName = UIdomotica->GetNodeList()->currentItem()->text();
        int nodeAddressDec = nodes->getNodeAddress(nodeName);
        if(nodeAddressDec == 1) { //Makes sure the provisioner wont get deleted
            return;
        }
        nodes->removeNode(nodeName);
        qDebug() << "Address: " << nodeAddressDec;
        QString nodeAddressHex = HEX_PREFIX + QString::number((nodeAddressDec), 16).rightJustified(4, '0');
        getState = true;
        WriteToComport(MESH_TARGET + nodeAddressHex);
        QTimer::singleShot(100, this, [this](){
            qDebug() << "Timer has passed: send remove node";
            WriteToComport(MESH_DELETE);
        });
    } else if (COMPORT == nullptr) {
        qDebug() << "Connect to a device first";
    }
}

void Comport::AddNewNode(QString uuid)
{
    int FreeNodeAddress = nodes->FirstFreeNodeAddress();
    QString nodeName = HEX_PREFIX + QString::number((FreeNodeAddress), 16).rightJustified(4, '0');

    nodes->addNode(nodeName, FreeNodeAddress);
    nodes->addActuatorToNode(nodeName, STRING_TO_SHOW_LED + QString(NODE_SHOW) + nodeName);
    nodes->addSensorToNode(nodeName, STRING_TO_SHOW_BUTTON + QString(NODE_SHOW) + nodeName);

    //all commands
    QTimer::singleShot(50, this, [this, uuid, nodeName](){
        qDebug() << "Timer has passed: add new node";
        WriteToComport(MESH_ADD_NODE + uuid + MESH_NETWORK_KEY + nodeName + " " + QString::number(ADD_NODE_DURATION));
    });
    QTimer::singleShot(10250, this, [this, nodeName](){
        qDebug() << "Timer has passed: target new node";
        WriteToComport(MESH_TARGET + nodeName);
    });
    QTimer::singleShot(10300, this, [this](){
        qDebug() << "Timer has passed: appkey create";
        WriteToComport(MESH_APP_KEY_CREATE);
    });
    QTimer::singleShot(10400, this, [this, nodeName](){
        qDebug() << "Timer has passed: appkey bind 1";
        WriteToComport(MESH_APP_KEY_BIND + nodeName + MESH_APP_KEY + MESH_SERVER);
    });
    QTimer::singleShot(10550, this, [this, nodeName](){
        qDebug() << "Timer has passed: appkey bind 1";
        WriteToComport(MESH_APP_KEY_BIND + nodeName + MESH_APP_KEY + MESH_CLIENT);
    });
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
            Data_From_SerialPort.remove("\u001B[1;32muart:~$").remove("\u001B[m").remove("\u001B[8D").remove("\u001B[0m");//.remove(Data_From_SerialPort.indexOf("\u001B[J"), 10);
            Data_From_SerialPort.remove("\u001B{");
            Data_From_SerialPort.remove(CONNECTED_OUT).remove(MESH_CREATE);
            Is_Data_Recieved = false;

            if(Data_From_SerialPort.contains("net_buf"))
            {
                Data_From_SerialPort = "";
                return;
            }


            qDebug() << "Message recieved: " << Data_From_SerialPort;

            // Add a new node
            QString uuid = inputChecks.CheckForUuid(Data_From_SerialPort);

            if (!uuid.isEmpty()){
                bool alreadyExist = false;
                for (const QString &tempUuid : uuidList) {
                    if(tempUuid == uuid)
                    {
                        alreadyExist = true;
                    }
                }
                if(!alreadyExist){
                    qDebug() << "new uuid found and trying to add node now";
                    AddNewNode(uuid);
                    uuidList.append(uuid);
                }
            }

            // Get the state after request
            if(getState)
            {
                QString newState = inputChecks.CheckForStatus(Data_From_SerialPort);
                qDebug() << "newState: " << newState;
                if(newState.contains("on")) {
                    UIdomotica->GetComputerNode()->setStyleSheet("background-color: yellow;");
                    getState = false;
                    QTimer::singleShot(2000, this, [this](){
                        UIdomotica->GetComputerNode()->setStyleSheet("background-color: grey;");
                    });
                } else if (newState.contains("off")){
                    UIdomotica->GetComputerNode()->setStyleSheet("background-color: black;");
                    getState = false;
                    QTimer::singleShot(2000, this, [this](){
                        UIdomotica->GetComputerNode()->setStyleSheet("background-color: grey;");
                    });
                }
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
