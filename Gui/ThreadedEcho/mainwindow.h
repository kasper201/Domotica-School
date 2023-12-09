#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QThread>
#include <QTextEdit>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class SerialPortReader : public QObject
{

    Q_OBJECT

public:
    explicit SerialPortReader(QSerialPort *serialPort, QObject *parent = nullptr);

public slots:
    void readSerialPort();

signals:
    void dataReceived(const QString &data);

private:
    QSerialPort *serialPort;
};

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateTextBox(const QString &data);

private:
    void setupUI();
    void setupSerialPort();
    void setupWorkerThread();

    Ui::MainWindow *ui;
    QWidget *centralWidget;
    QTextEdit *textEdit;
    QSerialPort *serialPort;
    QThread workerThread;
};

#endif // MAINWINDOW_H
