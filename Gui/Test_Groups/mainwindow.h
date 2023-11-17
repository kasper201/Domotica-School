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
#include <QThread>

#include "node.h"
#include "stringmodifiers.h"
#include "groups.h"

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

    void readData();

    void on_pushButton_Refresh_clicked();

    void addNodes();
    void closeConnection();

    void on_listWidget_Nodes_itemClicked(QListWidgetItem *item);
    void on_pushButton_Add_Sensor_Group_clicked();
    void addTitles(bool nodeNotNeeded);

    void on_pushButton_Add_Group_clicked();
    void on_pushButton_Delete_Group_clicked();

private:
    Ui::MainWindow *ui;

    //Class defines
    Node node;
    StringModifiers stringM;
    Groups groups;

    //Sending and recieving data
    QSerialPort* comport;
    QString Data_From_SerialPort;
    bool Is_Data_Recieved = false;

    //Items in ui
    QTabWidget* tabs;

    //connect to comport
    bool isComportConnected = false;
    QListWidget* comportList;
    QPushButton* connectComport;
    QPushButton* refreshComport;

    //send data over uart
    QLabel* comLabel;
    QListWidget* nodeList;
    QListWidget* sensorList;
    QListWidget* actuatorList;
    QListWidget* groupLinkList;
    QPushButton* sensorAddButton;

    //shows group
    QListWidget* groupList;
    QLineEdit* addGroupLine;
    QPushButton* addGroupButton;

    //Important String for comunication
    QString connected = "connected";            //Lets the node know that it is connected to the application
    QString disconnected = "disconnected";      //Lets the node know that the application is disconnected

};
#endif // MAINWINDOW_H
