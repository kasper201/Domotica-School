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


void Domotica::on_GroupList_currentRowChanged(int currentRow)
{
    if(currentRow >= 0)
    {
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

