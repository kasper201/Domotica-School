#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
    setupSerialPort();
    setupWorkerThread();
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
    delete ui;
}

SerialPortReader::SerialPortReader(QSerialPort *serialPort, QObject *parent)
    : QObject(parent), serialPort(serialPort)
{
    connect(serialPort, &QSerialPort::readyRead, this, &SerialPortReader::readSerialPort);
}

void SerialPortReader::readSerialPort()
{
    QByteArray data = serialPort->readAll();
    emit dataReceived(QString(data));
}

void MainWindow::updateTextBox(const QString &data)
{
    textEdit->append(data);
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    textEdit = new QTextEdit(this);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(textEdit);
}

void MainWindow::setupSerialPort()
{
    serialPort = new QSerialPort(this);
    serialPort->setPortName("COM1");  // Pas dit aan naar de juiste poort
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->open(QIODevice::ReadOnly);
}

void MainWindow::setupWorkerThread()
{
    SerialPortReader *serialPortReader = new SerialPortReader(serialPort);
    connect(serialPortReader, &SerialPortReader::dataReceived, this, &MainWindow::updateTextBox);

    serialPortReader->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, serialPortReader, &QObject::deleteLater);

    workerThread.start();
}

