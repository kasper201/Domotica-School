#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QLabel>

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
    void on_pushButton_Send_clicked();
    void readData();

private:
    Ui::MainWindow *ui;
    QSerialPort* COMPORT;
    QString Data_From_SerialPort;
    bool Is_Data_Recieved = false;

    //COMPORT!!
    QString Current_Comport = "COM3";

    //Items in ui
    QLabel* label_Data_Recieved = new QLabel;

};
#endif // MAINWINDOW_H
