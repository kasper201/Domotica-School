#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "portsetup.h"
#include "node.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    comportList = ui->listWidget_Comport;   //list of available comports
    connectComport = ui->pushButton_Connect;//Connect to the selected comport
    refreshComport = ui->pushButton_Refresh;//Refreshes the comport list

    dataLabel = ui->label_Data_Recieved;    //label that shows uart string
    sendButton = ui->pushButton_Send;       //Button that sends the data to uart
    dataOutput = ui->lineEdit_Serial_Data;  //Line with the data to send over uart
    comLabel = ui->label_Comport;           //Label that shows the current comport
    reconnectComport = ui->pushButton_Reconnect;//goes back so you can choose the comport again

    nodeAddState = 0;
    typeKnown = 0;
    preventMoreNodeNames = true;

    setupComportList();

    dataLabel->setText("");

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
    dataLabel->hide();
    sendButton->hide();
    dataOutput->hide();
    comLabel->hide();
    reconnectComport->hide();
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

    dataLabel->show();
    sendButton->show();
    dataOutput->show();
    comLabel->show();
    reconnectComport->show();
}


//Sends data to the comport
void MainWindow::on_pushButton_Send_clicked()
{
    if(comport->isOpen())
    {
        //char(10) = \n
        //char(13) = \r
        comport->write(dataOutput->text().toLatin1() + char(10) );
        comport->flush();
    }
}

//Goes back to the connect options
void MainWindow::on_pushButton_Reconnect_clicked()
{
    closeConnection();
    comportList->clear();
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

            addNodeAndActuators();

            Data_From_SerialPort = "";
        }
    }
}

void MainWindow::addNodeAndActuators()
{
    if(Data_From_SerialPort.contains("AddNode") && nodeAddState == 0) //Start the process of adding a new node
    {
        nodeAddState = 1; //add node
    }  else if(nodeAddState == 1)        //Adds node name
    {
        //Adds node name to class
        Data_From_SerialPort.remove("\r").remove("\n");

        if(preventMoreNodeNames)
        {
            node.addNodeInstance(Data_From_SerialPort);
            nodeName = Data_From_SerialPort;
            preventMoreNodeNames = false;
        }
        if(Data_From_SerialPort.contains("AddSensor"))
        {
            nodeAddState = 2; //add sensors
            preventMoreNodeNames = true;
        }
    } else if(nodeAddState == 2 && !Data_From_SerialPort.contains("AddSensor"))               //Adds sensors to node
    {
        //Adds sensors to map with same node name
        Data_From_SerialPort.remove("\r").remove("\n");
        if(typeKnown == 0 && !Data_From_SerialPort.contains("AddSensor"))
        {
            sensorType = Data_From_SerialPort;
            typeKnown = 1;
            Data_From_SerialPort = "";
        } else if(typeKnown == 1)
        {
            sensorName = Data_From_SerialPort;
            typeKnown = 2;
            node.addSensor(nodeName, sensorType, sensorName);
        }
        dataLabel->setText(node.getSensors(nodeName, sensorType));

        //After adding all sensors
        if(Data_From_SerialPort.contains("AddActuator"))
        {
            nodeAddState = 3;
            typeKnown = 0;
        }
    } else if(nodeAddState == 3 && !Data_From_SerialPort.contains("AddActuator"))               //Adds actuators to node
    {
        //Adds actuators to map with same node name
        Data_From_SerialPort.remove("\r").remove("\n");
        if(typeKnown == 0 && !Data_From_SerialPort.contains("AddActuator"))
        {
            actuatorType = Data_From_SerialPort;
            typeKnown = 1;
            Data_From_SerialPort = "";
        } else if(typeKnown == 1)
        {
            actuatorName = Data_From_SerialPort;
            typeKnown = 2;
            node.addActuator(nodeName, actuatorType, actuatorName);
        }
        dataLabel->setText(node.getActuators(nodeName, sensorType));

        //After adding all actuators go back to 0 so another node can be added
        if(typeKnown == 2)
        {
            QStringList nodeNames = node.getAllNodeNames();
            qDebug() << "Contents of QStringList:";
            for (const QString& element : nodeNames)
            {
                qDebug() << element;
            }
            nodeAddState = 0;
            typeKnown = 0;
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
        //QThread::msleep(100); Does not do much but could help with getting disconnected accros
        comport->close();
    }
}
