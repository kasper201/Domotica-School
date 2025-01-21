#ifndef DOMOTICA_H
#define DOMOTICA_H

#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class Domotica; }
QT_END_NAMESPACE

class Domotica : public QMainWindow
{
    Q_OBJECT

public:
    explicit Domotica(QWidget *parent = nullptr);
    ~Domotica();

    //For comport
    QListWidget* GetComportList();
    QLabel* GetComportLabel();
    QPushButton* GetConnectButton();

    //For Groups
    QListWidget* GetGroupList();
    QLineEdit* GetGroupName();
    QListWidget* GetGroupSensorsList();
    QListWidget* GetGroupActuatorsList();

    //For Nodes
    QListWidget* GetNodeList();
    QListWidget* GetNodeActuatorsList();
    QListWidget* GetNodeSensorsList();

signals:
    void comportConnectionRequested();          // Signal to notify Comport
    void comportRefreshRequested();             // Signal to notify Comport
    void groupAddRequested();                   // Signal to notify Groups
    void groupDeleteRequested();                // Signal to notify Groups
    void groupSensorAdd();                      // Signal to notify Groups
    void groupActuatorAdd();                    // Signal to notify Groups
    void groupSensorDeleteRequested();          // Signal to notify Groups
    void groupActuatorDeleteRequested();        // Signal to notify Groups
    void updateGroupParts(QString groupName);   // Signal to notify Groups
    void updateNodeParts(QString nodeName);     // Signal to notify Nodes

private slots:
    void on_ComportConnection_clicked();
    void on_ComportRefresh_clicked();
    void on_GroupAdd_clicked();
    void on_GroupDelete_clicked();

    void on_GroupList_currentRowChanged(int currentRow);
    void on_NodesList_currentRowChanged(int currentRow);

    void on_NodeAddActuator_clicked();
    void on_NodeAddSensor_clicked();

    void on_GroupActuatorDelete_clicked();

    void on_GroupSensorDelete_clicked();

private:
    Ui::Domotica *ui;

};
#endif // DOMOTICA_H
