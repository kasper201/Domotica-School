#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "portsetup.h"
#include "ledtoggle.h"

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

    yellowOn = ui->pushButton_Yellow_On;
    yellowOff = ui->pushButton_Yellow_Off;
    blueOn = ui->pushButton_Blue_On;
    blueOff = ui->pushButton_Blue_Off;
    redOn = ui->pushButton_Red_On;
    redOff = ui->pushButton_Red_Off;
    greenOn = ui->pushButton_Green_On;
    greenOff = ui->pushButton_Green_Off;


    setupComportList();

    dataLabel->setText("");

}

MainWindow::~MainWindow()
{
    comport->close();
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

    yellowOn->hide();
    yellowOff->hide();
    blueOn->hide();
    blueOff->hide();
    redOn->hide();
    redOff->hide();
    greenOn->hide();
    greenOff->hide();
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
    QString firstSixCharacters = comString.left(6);
    QString comPortName =firstSixCharacters.replace(" ", "");

    //connects to the right comport and puts all the setting right
    PortSetup portSetup(comPortName);
    comport = portSetup.COMPORT;
    connect(comport, SIGNAL(readyRead()), this, SLOT(readData()));

    //Puts the right Com into comLabel
    QString comportDescription = comportList->currentItem()->text();
    comLabel->setText("Comport: " + comportDescription);

    //update ui
    comportList->hide();
    connectComport->hide();
    refreshComport->hide();

    dataLabel->show();
    sendButton->show();
    dataOutput->show();
    comLabel->show();
    reconnectComport->show();

    yellowOn->show();
    yellowOff->show();
    blueOn->show();
    blueOff->show();
    redOn->show();
    redOff->show();
    greenOn->show();
    greenOff->show();
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
    comport->close();
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
            dataLabel->setText(Data_From_SerialPort);
            Data_From_SerialPort = "";
            Is_Data_Recieved = false;
        }
    }
}

//Turns yellow led on
void MainWindow::on_pushButton_Yellow_On_clicked()
{
    LedToggle ledToggle;
    comport->write(ledToggle.sendLedToggle("Yellow", true).toLatin1() + char(10) );
}

//Turns yellow led off
void MainWindow::on_pushButton_Yellow_Off_clicked()
{
    LedToggle ledToggle;
    comport->write(ledToggle.sendLedToggle("Yellow", false).toLatin1() + char(10) );
}

//Turns blue led on
void MainWindow::on_pushButton_Blue_On_clicked()
{
    LedToggle ledToggle;
    comport->write(ledToggle.sendLedToggle("Blue", true).toLatin1() + char(10) );
}

//Turns blue led off
void MainWindow::on_pushButton_Blue_Off_clicked()
{
    LedToggle ledToggle;
    comport->write(ledToggle.sendLedToggle("Blue", false).toLatin1() + char(10) );
}

//Turns red led on
void MainWindow::on_pushButton_Red_On_clicked()
{
    LedToggle ledToggle;
    comport->write(ledToggle.sendLedToggle("Red", true).toLatin1() + char(10) );
}

//Turns red led off
void MainWindow::on_pushButton_Red_Off_clicked()
{
    LedToggle ledToggle;
    comport->write(ledToggle.sendLedToggle("Red", false).toLatin1() + char(10) );
}

//Turns green led on
void MainWindow::on_pushButton_Green_On_clicked()
{
    LedToggle ledToggle;
    comport->write(ledToggle.sendLedToggle("Green", true).toLatin1() + char(10) );
}

//Turns green led off
void MainWindow::on_pushButton_Green_Off_clicked()
{
    LedToggle ledToggle;
    comport->write(ledToggle.sendLedToggle("Green", false).toLatin1() + char(10) );
}
