#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "portsetup.h"
#include "node.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(600, 600);

    tabs = ui->tabWidget;                           //tab widget so everything can be selected

    comportList = ui->listWidget_Comport;           //list of available comports
    connectComport = ui->pushButton_Connect;        //Connect to the selected comport
    refreshComport = ui->pushButton_Refresh;        //Refreshes the comport list

    comLabel = ui->label_Comport;                   //Label that shows the current comport
    nodeList = ui->listWidget_Nodes;                //List Widget with all nodes inside
    sensorList = ui->listWidget_Sensors;            //List Widget with all sensors of selected node
    actuatorList = ui->listWidget_Actuators;        //List Widget with all actuators of selected node
    groupLinkList = ui->listWidget_Link_Groups;     //List Widget with all groups
    sensorAddButton = ui->pushButton_Add_Sensor_Group; //Adds sensor to a group

    groupList = ui->listWidget_Groups;              //List Widget with all groups
    addGroupLine = ui->lineEdit_Add_Groups;         //Line with group name
    addGroupButton = ui->pushButton_Add_Group;      //Button to add group name

    setupComportList();
    addTitles(false);
    tabs->tabBar()->setTabEnabled(1, false);
    tabs->tabBar()->setTabEnabled(2, false);
}

MainWindow::~MainWindow()
{
    closeConnection();
    delete ui;
}

//sets up the listWidget with all available comport names
void MainWindow::setupComportList()
{
    //Puts all comports in a list
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        QString portName = port.portName();
        portName = portName.leftJustified(8, ' ');
        QString portDescription = port.description();
        QString portCombined = portName + "| " + portDescription;
        comportList->addItem(portCombined);
    }

    //hide for cuurently not needed elements
    sensorList->hide();
    actuatorList->hide();
    groupLinkList->hide();
    sensorAddButton->hide();
}

//Refreshes the listWidget with comports
void MainWindow::on_pushButton_Refresh_clicked()
{
    comportList->clear();
    setupComportList();
}

//Connect to the comport
void MainWindow::on_pushButton_Connect_clicked()
{
    if(isComportConnected == false && !comportList->selectedItems().isEmpty())
    {
        QString comString = comportList->currentItem()->text();
        QString firstSixCharacters = comString.left(4);
        QString comPortName =firstSixCharacters.replace(" ", "");

        //connects to the right comport and puts all the setting right
        PortSetup portSetup(comPortName);
        comport = portSetup.COMPORT;
        connect(comport, SIGNAL(readyRead()), this, SLOT(readData()));

        //Puts the right Com into comLabel
        QString comportDescription = comportList->currentItem()->text();
        comLabel->setText("Comport: " + comportDescription);

        tabs->tabBar()->setTabEnabled(1, true);
        tabs->tabBar()->setTabEnabled(2, true);
        tabs->setCurrentIndex(1);

        //Writes connected to dongle
        QString wakeUp = "WakeupArduino";
        comport->write(wakeUp.toLatin1() + char(10) );
        comport->write(connected.toLatin1() + char(10) );

        connectComport->setText("Disconnect");
        isComportConnected = true;
    } else if(isComportConnected == true)
    {
        closeConnection();
        comportList->clear();
        nodeList->clear();
        addTitles(false);
        setupComportList();

        tabs->tabBar()->setTabEnabled(1, false);
        tabs->tabBar()->setTabEnabled(2, false);
        tabs->setCurrentIndex(0);

        comLabel->setText("Not connected");
        connectComport->setText("Connect");
        isComportConnected = false;
    } else if (isComportConnected == false && comportList->selectedItems().isEmpty())
    {
        comLabel->setText("A comport should be selected first");
        qDebug() << "No comport was selected";
    }else
    {
        qDebug() << "Failed with connection";
    }
}

//Reads data from the comport
void MainWindow::readData()
{
    if(comport->isOpen())
    {
        //Reads uart
        while(comport->bytesAvailable())
        {
            Data_From_SerialPort += comport->readAll();

            //Checks if the line has ended
            if(Data_From_SerialPort.at(Data_From_SerialPort.length() - 1) == char(10))
            {
                Is_Data_Recieved = true;
            }
        }

        //turns data recieved off again
        if(Is_Data_Recieved == true)
        {
            qDebug() << "Data from serial port: " << Data_From_SerialPort;
            //dataLabel->setText(Data_From_SerialPort);
            Is_Data_Recieved = false;

            addNodes();

            Data_From_SerialPort = "";
        }
    }
}

void MainWindow::addNodes()
{
    if(Data_From_SerialPort.contains("AddNode")) //Start the process of adding a new node
    {
        //Removes parts of the string that are irrelevant
        Data_From_SerialPort.remove("\r").remove("\n");
        Data_From_SerialPort = stringM.removedTillWhitespace(Data_From_SerialPort);

        //Adds a nodename
        QString nodeName = stringM.removedFromWhitespace(Data_From_SerialPort);
        node.addNodeInstance(nodeName);
        nodeList->addItem(nodeName);
        Data_From_SerialPort = stringM.removedTillWhitespace(Data_From_SerialPort);

        //Adds all sensors
        while(Data_From_SerialPort.contains("AddSensor"))
        {
            Data_From_SerialPort = stringM.removedTillWhitespace(Data_From_SerialPort);
            QString sensorType = stringM.removedFromWhitespace(Data_From_SerialPort);
            Data_From_SerialPort = stringM.removedTillWhitespace(Data_From_SerialPort);
            QString sensorName = stringM.removedFromWhitespace(Data_From_SerialPort);
            Data_From_SerialPort = stringM.removedTillWhitespace(Data_From_SerialPort);
            node.addSensor(nodeName, sensorType, sensorName);
        }

        //Adds all actuators
        while(Data_From_SerialPort.contains("AddActuator"))
        {
            Data_From_SerialPort = stringM.removedTillWhitespace(Data_From_SerialPort);
            QString actuatorType = stringM.removedFromWhitespace(Data_From_SerialPort);
            Data_From_SerialPort = stringM.removedTillWhitespace(Data_From_SerialPort);
            QString actuatorName = stringM.removedFromWhitespace(Data_From_SerialPort);
            Data_From_SerialPort = stringM.removedTillWhitespace(Data_From_SerialPort);
            QString actuatorStatus = stringM.removedFromWhitespace(Data_From_SerialPort);
            Data_From_SerialPort = stringM.removedTillWhitespace(Data_From_SerialPort);
            node.addActuator(nodeName, actuatorType, actuatorName, actuatorStatus);
        }
    }
}

//closes the comport connection on program shutdown or reconnect
void MainWindow::closeConnection()
{
    if(comport->isOpen())
    {
        comport->write(disconnected.toLatin1() + char(10) );
        qDebug() << "Closed comport connection: " << disconnected;
        comport->close();
    }
}

//Shows all relevant information next to the node+
void MainWindow::on_listWidget_Nodes_itemClicked(QListWidgetItem *item)
{
    //Clears List for this click
    sensorList->clear();
    actuatorList->clear();
    groupLinkList->clear();

    addTitles(true);
    //fills sensorList
    QStringList nodeSensors = node.getAllSensorNames(item->text());
    for (const QString& element : nodeSensors)
    {
        sensorList->addItem(element);
    }

    //fills actuatorList
    QStringList nodeActuators = node.getAllActuatorNames(item->text());
    for (const QString& element : nodeActuators)
    {
        QString actuatorStatus = QString::fromUtf8(node.getActuatorStatus(item->text(), element) ? "true" : "false");
        actuatorList->addItem(element + "\t" + actuatorStatus);
    }

    QStringList allGroups = groups.getGroups();
    foreach (const QString &groupName, allGroups) {
        groupLinkList->addItem(groupName);
    }
    sensorList->show();
    actuatorList->show();
    if(groupLinkList->count() > 1)
    {
        groupLinkList->show();
        sensorAddButton->show();
    }
}

//adds titles to all listWidgets on node page
void MainWindow::addTitles(bool nodeNotNeeded)
{
    QListWidgetItem *headerItem = new QListWidgetItem("Nodes");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    if(nodeNotNeeded == false)
    {
        nodeList->addItem(headerItem);
    }
    headerItem = new QListWidgetItem("Sensor");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    sensorList->addItem(headerItem);
    headerItem = new QListWidgetItem("Actuator\tStatus");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    actuatorList->addItem(headerItem);
    headerItem = new QListWidgetItem("Groups");
    headerItem->setFont(QFont("Arial", 12, QFont::Bold));
    groupLinkList->addItem(headerItem);
}

void MainWindow::on_pushButton_Add_Group_clicked()
{
    groups.addGroupInstance(addGroupLine->text());
    groupList->addItem(addGroupLine->text());
    addGroupLine->clear();
}


void MainWindow::on_pushButton_Delete_Group_clicked()
{
    groups.deleteGroupInstance(groupList->currentItem()->text());
    groupList->takeItem(groupList->currentRow());
}


//Adds a sensor to a group
void MainWindow::on_pushButton_Add_Sensor_Group_clicked()
{
    QString groupName = groupLinkList->currentItem()->text();
    QString nodeName = nodeList->currentItem()->text();
    QString sensorName = sensorList->currentItem()->text();

    groups.addSensor(groupName, nodeName, "Button", sensorName);
}

