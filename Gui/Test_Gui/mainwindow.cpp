#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    }
    else {
        qDebug() << "Serial Port is not connected";
        qDebug() << COMPORT->error();
    }

    connect(COMPORT, SIGNAL(readyRead()), this, SLOT(readData()));
    ui->label_Data_Recieved->setText("");

}

void MainWindow::on_pushButton_Send_clicked()
{
    if(COMPORT->isOpen())
    {
        //char(10) = \n
        //char(13) = \r
        COMPORT->write(ui->lineEdit_Serial_Data->text().toLatin1() + char(10) );
        COMPORT->flush();
    }
}

void MainWindow::readData()
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
            qDebug() << "Data from serial port: " << Data_From_SerialPort;
            ui->label_Data_Recieved->setText(Data_From_SerialPort);
            Data_From_SerialPort = "";
            Is_Data_Recieved = false;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
