#include "domotica.h"
#include "ui_domotica.h"

Domotica::Domotica(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Domotica)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::darkCyan);
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setFixedSize(1280, 720);
    this->resize(1280, 720);

    ui->setupUi(this);
    QPalette framePalette = ui->frame->palette();
    framePalette.setColor(QPalette::Window, Qt::lightGray);
    ui->ComportFrame->setPalette(framePalette);
    ui->ComportFrame->setAutoFillBackground(true);
    ui->GroupOverview->setPalette(framePalette);
    ui->GroupListView->setAutoFillBackground(true);
    ui->GroupListView->setPalette(framePalette);
    ui->GroupOverview->setAutoFillBackground(true);
    ui->NodeOverview->setPalette(framePalette);
    ui->NodeOverview->setAutoFillBackground(true);
    ui->ApplicationBackground->setPalette(framePalette);
    ui->ApplicationBackground->setAutoFillBackground(true);

    QString buttonStyle = "QPushButton{"
                          "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
                          "stop: 0 white, stop: 1 white);"
                          "border-style: solid;"
                          "border-width: 0px;"
                          "border-color: black;"
                          "border-radius: 15px;"
                          "padding: 3px 3px 3px 3px;"
                          "}";

    ui->ComportConnection->setStyleSheet(buttonStyle);
    ui->ComportRefresh->setStyleSheet(buttonStyle);
    ui->GroupAdd->setStyleSheet(buttonStyle);
    ui->GroupDelete->setStyleSheet(buttonStyle);
    ui->GroupActuatorDelete->setStyleSheet(buttonStyle);
    ui->GroupSensorDelete->setStyleSheet(buttonStyle);
    ui->NodeAddActuator->setStyleSheet(buttonStyle);
    ui->NodeAddSensor->setStyleSheet(buttonStyle);
    ui->ApplicationSensor->setStyleSheet(buttonStyle);

    ui->ApplicationActuator->setStyleSheet("background-color: black;");
}

Domotica::~Domotica()
{
    delete ui;
}

//For comport
QListWidget* Domotica::GetComportList()
{
    return ui->ComportList;
}

//For comport
QLabel* Domotica::GetComportLabel()
{
    return ui->ComportStatus;
}

//For comport
QPushButton* Domotica::GetConnectButton()
{
    return ui->ComportConnection;
}

//For Groups
QListWidget* Domotica::GetGroupList()
{
    return ui->GroupList;
}

//For Groups
QLineEdit* Domotica::GetGroupName()
{
    return ui->GroupName;
}

//For Groups
QListWidget* Domotica::GetGroupSensorsList()
{
    return ui->GroupSensorsList;
}

//For Groups
QListWidget* Domotica::GetGroupActuatorsList()
{
    return ui->GroupActuatorsList;
}

//For Nodes
QListWidget* Domotica::GetNodeList()
{
    return ui->NodesList;
}

//For Nodes
QListWidget* Domotica::GetNodeActuatorsList()
{
    return ui->NodeActuatorsList;
}

//For Nodes
QListWidget* Domotica::GetNodeSensorsList()
{
    return ui->NodeSensorsList;
}

QListWidget* Domotica::GetComputerNode()
{
    return ui->ApplicationActuator;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Signals underneath
///
void Domotica::on_ComportConnection_clicked()
{
    emit comportConnectionRequested();
}


void Domotica::on_ComportRefresh_clicked()
{
    emit comportRefreshRequested();
}


void Domotica::on_GroupAdd_clicked()
{
    emit groupAddRequested();
}


void Domotica::on_GroupDelete_clicked()
{
    emit groupDeleteRequested();
}

void Domotica::on_GroupSensorDelete_clicked()
{
    emit groupSensorDeleteRequested();
}


void Domotica::on_GroupActuatorDelete_clicked()
{
    emit groupActuatorDeleteRequested();
}

void Domotica::on_GroupList_currentRowChanged(int currentRow)
{
    if(currentRow >= 0)
    {
        emit updateGroupParts(ui->GroupList->currentItem()->text());
        ui->GroupActuatorsLabel->setText("Actuators from group: " + ui->GroupList->currentItem()->text());
        ui->GroupSensorsLabel->setText("Sensors from group: " + ui->GroupList->currentItem()->text());
    }
}


void Domotica::on_NodesList_currentRowChanged(int currentRow)
{
    if(currentRow >= 0)
    {
        QString nodeName = ui->NodesList->currentItem()->text();
        emit updateNodeParts(nodeName);
    }
}


void Domotica::on_NodeAddActuator_clicked()
{
    emit groupActuatorAdd();
}


void Domotica::on_NodeAddSensor_clicked()
{
    emit groupSensorAdd();
}

void Domotica::on_ApplicationSensor_clicked()
{
    emit sendOutComputer();
}

