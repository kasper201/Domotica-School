#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QLabel>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupComportList();
    void on_pushButton_Connect_clicked();

    void on_pushButton_Send_clicked();
    void on_pushButton_Reconnect_clicked();
    void readData();

    void on_pushButton_Refresh_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort* comport;
    QString Data_From_SerialPort;
    bool Is_Data_Recieved = false;

    //Items in ui
    QLabel* label_Data_Recieved = new QLabel;

    //Replacing ui->...
    //connect to comport
    QListWidget* comportList;
    QPushButton* connectComport;
    QPushButton* refreshComport;

    //send data over uart
    QLabel* dataLabel;
    QPushButton* sendButton;
    QLineEdit* dataOutput;
    QLabel* comLabel;
    QPushButton* reconnectComport;

    //Led on or off
    bool yellow;
    bool blue;
    bool red;
    bool green;

};
#endif // MAINWINDOW_H
