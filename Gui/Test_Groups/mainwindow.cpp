#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "portsetup.h"
#include "node.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    comportList = ui->listWidget_Comport;           //list of available comports
    connectComport = ui->pushButton_Connect;        //Connect to the selected comport
    refreshComport = ui->pushButton_Refresh;        //Refreshes the comport list

    comLabel = ui->label_Comport;                   //Label that shows the current comport
    reconnectComport = ui->pushButton_Reconnect;    //goes back so you can choose the comport again
    nodeList = ui->listWidget_Nodes;                //List Widget with all nodes inside
    sensorList = ui->listWidget_Sensors;            //List Widget with all sensors of selected node
    actuatorList = ui->listWidget_Actuators;        //List Widget with all actuators of selected node

    setupComportList();
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

    //Shows current elements
    comportList->show();
    connectComport->show();
    refreshComport->show();

    //hide for cuurently not needed elements
    comLabel->hide();
    reconnectComport->hide();
    nodeList->hide();
    sensorList->hide();
    actuatorList->hide();
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

    //Writes connected to dongle
    QString wakeUp = "WakeupArduino";
    comport->write(wakeUp.toLatin1() + char(10) );
    comport->write(connected.toLatin1() + char(10) );

    //update ui
    comportList->hide();
    connectComport->hide();
    refreshComport->hide();

    comLabel->show();
    reconnectComport->show();
    nodeList->show();
}

//Goes back to the connect options
void MainWindow::on_pushButton_Reconnect_clicked()
{
    closeConnection();
    comportList->clear();
    nodeList->clear();
    setupComportList();
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
        Data_From_SerialPort = removedTillWhitespace(Data_From_SerialPort);

        //Adds a nodename
        nodeName = removedFromWhitespace(Data_From_SerialPort);
        node.addNodeInstance(nodeName);
        nodeList->addItem(nodeName);
        Data_From_SerialPort = removedTillWhitespace(Data_From_SerialPort);

        //Adds all sensors
        while(Data_From_SerialPort.contains("AddSensor"))
        {
            Data_From_SerialPort = removedTillWhitespace(Data_From_SerialPort);
            QString sensorType = removedFromWhitespace(Data_From_SerialPort);
            Data_From_SerialPort = removedTillWhitespace(Data_From_SerialPort);
            QString sensorName = removedFromWhitespace(Data_From_SerialPort);
            Data_From_SerialPort = removedTillWhitespace(Data_From_SerialPort);
            node.addSensor(nodeName, sensorType, sensorName);
        }

        //Adds all actuators
        while(Data_From_SerialPort.contains("AddActuator"))
        {
            Data_From_SerialPort = removedTillWhitespace(Data_From_SerialPort);
            QString actuatorType = removedFromWhitespace(Data_From_SerialPort);
            Data_From_SerialPort = removedTillWhitespace(Data_From_SerialPort);
            QString actuatorName = removedFromWhitespace(Data_From_SerialPort);
            Data_From_SerialPort = removedTillWhitespace(Data_From_SerialPort);
            node.addActuator(nodeName, actuatorType, actuatorName);
        }
    }
}

QString MainWindow::removedTillWhitespace(QString string)
{
    int firstWhitespaceIndex; //where the first whitespace of a string is located
    firstWhitespaceIndex = string.indexOf(' ');

    // Check if a whitespace was found
    if (firstWhitespaceIndex != -1) {
        // Remove the substring from the beginning of the string up to the first whitespace
        string.remove(0, firstWhitespaceIndex + 1);
    }
    return string;
}

QString MainWindow::removedFromWhitespace(QString string)
{
    int firstWhitespaceIndex; //where the first whitespace of a string is located
    firstWhitespaceIndex = string.indexOf(' ');

    // Check if a whitespace was found
    if (firstWhitespaceIndex != -1) {
        // Remove the substring from the beginning of the string up to the first whitespace
        string.remove(firstWhitespaceIndex, string.length());
    }
    return string;
}

//closes the comport connection on program shutdown or reconnect
void MainWindow::closeConnection()
{
    if(comport->isOpen())
    {
        comport->write(disconnected.toLatin1() + char(10) );
        qDebug() << "Closed comport connection: " << disconnected;
        //QThread::msleep(100); Does not do much but could help with getting disconnected accros
        comport->close();
    }
}

void MainWindow::on_listWidget_Nodes_itemClicked(QListWidgetItem *item)
{
    //Clears List for this click
    sensorList->clear();
    actuatorList->clear();

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
        actuatorList->addItem(element);
    }

    sensorList->show();
    actuatorList->show();
}

