#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "portsetup.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connects to the right comport and puts all the setting right
    PortSetup portSetup;
    comport = portSetup.COMPORT;
    connect(comport, SIGNAL(readyRead()), this, SLOT(readData()));

    ui->label_Data_Recieved->setText("");

}

void MainWindow::on_pushButton_Send_clicked()
{
    if(comport->isOpen())
    {
        //char(10) = \n
        //char(13) = \r
        comport->write(ui->lineEdit_Serial_Data->text().toLatin1() + char(10) );
        comport->flush();
    }
}

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
