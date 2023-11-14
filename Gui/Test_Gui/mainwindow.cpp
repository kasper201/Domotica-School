#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "portsetup.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);




    comportList = ui->listWidget_Comport;   //list of available comports
    connectComport = ui->pushButton_Connect;//Connect to the selected comport

    dataLabel = ui->label_Data_Recieved;    //label that shows uart string
    sendButton = ui->pushButton_Send;       //Button that sends the data to uart
    dataOutput = ui->lineEdit_Serial_Data;  //Line with the data to send over uart

    //hide for now not needed elements
    dataLabel->hide();
    sendButton->hide();
    dataOutput->hide();

    //Puts all comports in a list
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        QString portName = port.portName();
        portName = portName.leftJustified(8, ' ');
        QString portDescription = port.description();
        QString portCombined = portName + "| " + portDescription;
        comportList->addItem(portCombined);
    }

    dataLabel->setText("");

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Connect to the comport
void MainWindow::on_pushButton_Connect_clicked()
{
    QString comString = comportList->currentItem()->text();
    QString firstSixCharacters = comString.left(6);
    QString comPortName =firstSixCharacters.replace(" ", "");

    //connects to the right comport and puts all the setting right
    PortSetup portSetup(comPortName);
    comport = portSetup.COMPORT;
    connect(comport, SIGNAL(readyRead()), this, SLOT(readData()));

    //update ui
    comportList->hide();
    connectComport->hide();

    dataLabel->show();
    sendButton->show();
    dataOutput->show();
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
            dataLabel->setText(Data_From_SerialPort);
            Data_From_SerialPort = "";
            Is_Data_Recieved = false;
        }
    }
}
