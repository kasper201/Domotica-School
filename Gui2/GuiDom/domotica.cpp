#include "domotica.h"
#include "ui_domotica.h"

Domotica::Domotica(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Domotica)
{
    ui->setupUi(this);
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

