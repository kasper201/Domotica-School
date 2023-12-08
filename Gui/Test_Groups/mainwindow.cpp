#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(600, 600);

    tabs = ui->tabWidget;                           //tab widget so everything can be selected

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::darkCyan);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QPalette tabPalette = ui->tab->palette();
    tabPalette.setColor(QPalette::Window, Qt::lightGray);
    ui->tab->setPalette(tabPalette);
    ui->tab->setAutoFillBackground(true);
    ui->tab_2->setPalette(tabPalette);
    ui->tab_2->setAutoFillBackground(true);
    ui->tab_3->setPalette(tabPalette);
    ui->tab_3->setAutoFillBackground(true);
    ui->tab_8->setPalette(tabPalette);
    ui->tab_8->setAutoFillBackground(true);

    //Comport tab
    comportList = ui->listWidget_Comport;           //list of available comports
    connectComport = ui->pushButton_Connect;        //Connect to the selected comport
    refreshComport = ui->pushButton_Refresh;        //Refreshes the comport list

    //Nodes tab
    comLabel = ui->label_Comport;                   //Label that shows the current comport
    nodeList = ui->listWidget_Nodes;                //List Widget with all nodes inside
    sensorList = ui->listWidget_Sensors;            //List Widget with all sensors of selected node
    actuatorList = ui->listWidget_Actuators;        //List Widget with all actuators of selected node
    groupLinkList = ui->listWidget_Link_Groups;     //List Widget with all groups
    sensorAddButton = ui->pushButton_Add_Sensor_Group;      //Adds sensor to a group
    actuatorAddButton = ui->pushButton_Add_Actuator_Group;  //Adds actuator to a group

    //Group tab
    groupList = ui->listWidget_Groups;              //List Widget with all groups
    addGroupLine = ui->lineEdit_Add_Groups;         //Line with group name
    addGroupButton = ui->pushButton_Add_Group;      //Button to add group name
    groupLabel = ui->label_Current_Group;           //Shows currently selected group
    sensorListGroup = ui->listWidget_Selected_Group_Sensors;    //Shows all sensors from current group
    actuatorListGroup = ui->listWidget_Selected_Group_Actuators;//Shows all actuators from current group
    deleteSensor = ui->pushButton_Delete_Sensor;    //Deletes selected sensor from selected group
    deleteActuator = ui->pushButton_Delete_Actuator;//Deletes selected actuator from selected group

    //Adds application node
    function.addNodes("AddNode Application_ AddSensor Button______ App_Button__ AddActuator LED_________ App_LED_____ false", node, nodeList);

    setupComportList();
    function.addTitles(false, nodeList, sensorList, actuatorList, node);
    tabs->tabBar()->setTabEnabled(1, false);
    tabs->tabBar()->setTabEnabled(2, false);
    tabs->tabBar()->setTabEnabled(3, false);
}

MainWindow::~MainWindow()
{
    closeConnection();
    delete ui;
}

//sets up the listWidget with all available comport names
void MainWindow::setupComportList()
{
    //Puts all comports in a list
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        QString portName = port.portName();
        portName = portName.leftJustified(8, ' ');
        QString portDescription = port.description();
        QString portCombined = portName + "| " + portDescription;
        comportList->addItem(portCombined);
    }

    //hide for cuurently not needed elements
    groupLinkList->hide();
    function.updateGroupLists(groupList, groupLinkList, sensorAddButton, actuatorAddButton, groups, nodeList);
}

//Refreshes the listWidget with comports
void MainWindow::on_pushButton_Refresh_clicked()
{
    comportList->clear();
    setupComportList();
}

//Connect to the comport
void MainWindow::on_pushButton_Connect_clicked()
{
    if(isComportConnected == false && !comportList->selectedItems().isEmpty())
    {
        QString comString = comportList->currentItem()->text();
        QString firstSixCharacters = comString.left(4);
        QString comPortName =firstSixCharacters.replace(" ", "");

        //connects to the right comport and puts all the setting right
        portSetup.setupComport(comPortName);
        comport = portSetup.COMPORT;
        connect(comport, SIGNAL(readyRead()), this, SLOT(readData()));

        //Puts the right Com into comLabel
        QString comportDescription = comportList->currentItem()->text();
        comLabel->setText("Comport: " + comportDescription);

        tabs->tabBar()->setTabEnabled(1, true);
        tabs->tabBar()->setTabEnabled(2, true);
        tabs->tabBar()->setTabEnabled(3, true);
        tabs->setCurrentIndex(1);
        ui->tabWidget_3->setCurrentIndex(0);

        //Writes connected to dongle
        QString wakeUp = "WakeupArduino";
        comport->write(wakeUp.toLatin1() + char(10) );
        comport->write(connected.toLatin1() + char(10) );

        connectComport->setText("Disconnect");
        isComportConnected = true;
    } else if(isComportConnected == true)
    {
        closeConnection();
        comportList->clear();
        nodeList->clear();
        function.addTitles(false, nodeList, sensorList, actuatorList, node);
        setupComportList();

        tabs->tabBar()->setTabEnabled(1, false);
        tabs->tabBar()->setTabEnabled(2, false);
        tabs->tabBar()->setTabEnabled(3, false);
        tabs->setCurrentIndex(0);

        comLabel->setText("Not connected");
        connectComport->setText("Connect");
        isComportConnected = false;
    } else if (isComportConnected == false && comportList->selectedItems().isEmpty())
    {
        comLabel->setText("A comport should be selected first");
        qDebug() << "No comport was selected";
    }else
    {
        qDebug() << "Failed with connection";
    }
}

//Reads data from the comport
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
            Data_From_SerialPort.remove("\r").remove("\n");
            Is_Data_Recieved = false;

            Data_From_SerialPort = function.addNodes(Data_From_SerialPort, node, nodeList);
            Data_From_SerialPort = function.addGroups(Data_From_SerialPort, groups, groupList);


            if(Data_From_SerialPort.contains("TriggerSensor"))
            {
                groupsTriggered = sensorInput.sensorTrigger(groups, Data_From_SerialPort);
            }

            if(Data_From_SerialPort.contains("UpdateAppActuator"))
            {
                sensorInput.actuatorUpdate(node, Data_From_SerialPort);
                updateNodeLists();
                actuatorsTriggered.removeOne(actuatorUpdate);
                actuatorUpdateLock = false;
            }

            Data_From_SerialPort = "";
        }
        //Allows a group update because there are no actuators of the last group left
        if(actuatorsTriggered.isEmpty())
        {
            groupUpdateLock = false;
        }


        foreach (const QString &groupName, groupsTriggered)   //Adds all groups to groupLinkList
        {
            qDebug() << groupName;
        }

        //Goes to the next group after the first one is updated
        if(!groupsTriggered.isEmpty() && groupUpdateLock == false)
        {
            QString groupName = groupsTriggered.first();
            actuatorsTriggered = sensorInput.groupTriggered(groups, node, groupName);
            qDebug() << groupName;
            groupsTriggered.removeOne(groupName);
            groupUpdateLock = true;
        }

        if(!actuatorsTriggered.isEmpty() && actuatorUpdateLock == false)
        {
            actuatorUpdate = actuatorsTriggered.first();
            if(!actuatorUpdate.contains("App_LED"))
            {
                qDebug() << actuatorUpdate;
                portSetup.WriteToComport(actuatorUpdate);
                actuatorUpdateLock = true;
            }
            if (actuatorUpdate.contains("App_LED"))
            {
                if(node.getActuatorStatus("Application", "App_LED") == "false")
                {
                    node.updateActuatorStatus("Application", "App_LED", "true");
                    ui->widget_led->setStyleSheet("background-color: yellow;");
                } else {
                    node.updateActuatorStatus("Application", "App_LED", "false");
                    ui->widget_led->setStyleSheet("background-color: black;");
                }
                actuatorsTriggered.removeOne(actuatorUpdate);
                qDebug() << node.getActuatorStatus("Application", "App_LED");
            }
        }
    }
}

//closes the comport connection on program shutdown or reconnect
void MainWindow::closeConnection()
{
    if(comport->isOpen())
    {
        comport->write(disconnected.toLatin1() + char(10) );
        qDebug() << "Closed comport connection: " << disconnected;
        comport->close();
    }
}

//Shows all relevant information next to the node+
void MainWindow::on_listWidget_Nodes_itemClicked()
{
    updateNodeLists();
    function.updateGroupLists(groupList, groupLinkList, sensorAddButton, actuatorAddButton, groups, nodeList);
}

//Adds a sensor to a group
void MainWindow::on_pushButton_Add_Sensor_Group_clicked()
{
    if(!groupLinkList->selectedItems().isEmpty())
    {
        QString groupName = groupLinkList->currentItem()->text();
        QString nodeName = nodeList->currentItem()->text();
        QString sensorName = sensorList->currentItem()->text().split('\t').value(0);
        QString sensorType = sensorList->currentItem()->text().split('\t').value(1);

        groups.addSensor(groupName, nodeName, sensorType, sensorName);
        //updateCurrentGroupOverview();
    } else
    {
        qDebug() << "A group should be selected first";
    }
}

//Adds a actuator to a group
void MainWindow::on_pushButton_Add_Actuator_Group_clicked()
{
    if(!groupLinkList->selectedItems().isEmpty())
    {
        QString groupName = groupLinkList->currentItem()->text();
        QString nodeName = nodeList->currentItem()->text();
        QString actuatorName = actuatorList->currentItem()->text().split('\t').value(0);
        QString actuatorType = actuatorList->currentItem()->text().split('\t').value(1);

        groups.addActuator(groupName, nodeName, actuatorType, actuatorName);
    } else
    {
        qDebug() << "A group should be selected first";
    }
}

//Adds group with name in lineEdit
void MainWindow::on_pushButton_Add_Group_clicked()
{
    groups.addGroupInstance(addGroupLine->text());
    QString CreateGroup = "CreateGroup " + addGroupLine->text();
    portSetup.WriteToComport(CreateGroup);
    function.updateGroupLists(groupList, groupLinkList, sensorAddButton, actuatorAddButton, groups, nodeList);
    addGroupLine->clear();
}

//Deletes selected group
void MainWindow::on_pushButton_Delete_Group_clicked()
{
    if(!groupList->selectedItems().isEmpty())
    {
        groups.deleteGroupInstance(groupList->currentItem()->text());
        groupList->takeItem(groupList->currentRow());
        function.updateCurrentGroupOverview(sensorListGroup, actuatorListGroup, groupList, groups);
        groupLabel->setText("Group has been deleted select another group");
    } else
    {
        qDebug() << "No group was selected";
    }
}

//Shows the information of selected group
void MainWindow::on_listWidget_Groups_itemClicked(QListWidgetItem *item)
{
    groupLabel->setText("Group: " + item->text());
    function.updateCurrentGroupOverview(sensorListGroup, actuatorListGroup, groupList, groups);
}

//Deletes a sensor from a group
void MainWindow::on_pushButton_Delete_Sensor_clicked()
{
    if(!sensorListGroup->selectedItems().isEmpty() && !groupList->selectedItems().isEmpty())
    {
        QString sensorName = sensorListGroup->currentItem()->text().split('\t').value(0);
        QString groupName = groupList->currentItem()->text();

        groups.deleteSensor(groupName, sensorName);
        sensorListGroup->takeItem(sensorListGroup->currentRow());
    } else if(sensorListGroup->selectedItems().isEmpty()) {
        qDebug() << "select a sensor";
    }
}

//Deletes a actuator from a group
void MainWindow::on_pushButton_Delete_Actuator_clicked()
{
    if(!actuatorListGroup->selectedItems().isEmpty() && !groupList->selectedItems().isEmpty())
    {
        QString actuatorName = actuatorListGroup->currentItem()->text().split('\t').value(0);
        QString groupName = groupList->currentItem()->text();

        groups.deleteActuator(groupName, actuatorName);
        actuatorListGroup->takeItem(actuatorListGroup->currentRow());
    } else if(actuatorListGroup->selectedItems().isEmpty()) {
        qDebug() << "select an actuator";
    }
}

//updates node sensor and node actuator lists
void MainWindow::updateNodeLists()
{

    //Clears List for this function
    sensorList->clear();
    actuatorList->clear();

    function.addTitles(true, nodeList, sensorList, actuatorList, node);
    //fills sensorList
    if(!nodeList->selectedItems().isEmpty())
    {
        QStringList nodeSensors = node.getAllSensorNames(nodeList->currentItem()->text());
        for (const QString& element : nodeSensors)
        {
            sensorList->addItem(element);
        }

        //fills actuatorList
        QStringList nodeActuators = node.getAllActuatorNames(nodeList->currentItem()->text());
        for (const QString& element : nodeActuators)
        {
            QString actuatorName = element.split('\t').value(0);
            QString actuatorStatus = node.getActuatorStatus(nodeList->currentItem()->text(), actuatorName);
            actuatorList->addItem(element + "\t" + actuatorStatus);
        }
    }
}

//Sets some of the tab widgets right for when the it is viewed
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index == 1)
    {
        ui->tabWidget_3->setCurrentIndex(0);
        function.updateCurrentGroupOverview(sensorListGroup, actuatorListGroup, groupList, groups);
    } else if(index == 2)
    {
        ui->tabWidget_2->setCurrentIndex(0);
        function.addTitles(false, nodeList, sensorList, actuatorList, node);
    } else if(index == 3)
    {
        if(node.getActuatorStatus("Application", "App_LED") == "true")
        {
            ui->widget_led->setStyleSheet("background-color: yellow;");
        } else {
            ui->widget_led->setStyleSheet("background-color: black;");
        }
    }
}

//Triggers when the application button is clicked
void MainWindow::on_appButton_clicked()
{
    groupsTriggered = sensorInput.sensorTrigger(groups, "TriggerSensor Application_ App_Button__");
    readData();
}

