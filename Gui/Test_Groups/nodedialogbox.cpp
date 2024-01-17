// nodedialogbox.cpp

#include "nodedialogbox.h"

NodeDialogBox::NodeDialogBox(const QString& nodeName, int whichPopUp, QWidget *parent)
    : QDialog(parent)
{
    //Decides which pop up is shown
    if(whichPopUp == 0)                                                                         //For adding the node address and element address to an existing node
    {
        setWindowTitle("Set Node properties for: " + nodeName);

        // Create input fields
        addressSpinBox = new QSpinBox(this);
        elementSpinBox = new QSpinBox(this);

        addressSpinBox->setRange(0, 1024);
        elementSpinBox->setRange(0, 1024);

        // Create buttons
        okButton = new QPushButton("OK", this);
        cancelButton = new QPushButton("Cancel", this);

        // Connect signals and slots for the buttons
        connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

        // Set up layout
        QGridLayout *mainLayout = new QGridLayout(this);
        mainLayout->addWidget(new QLabel("Node Address:"), 0, 0);
        mainLayout->addWidget(addressSpinBox, 0, 1);
        mainLayout->addWidget(new QLabel("Node Element:"), 1, 0);
        mainLayout->addWidget(elementSpinBox, 1, 1);

        // Add buttons to the layout
        mainLayout->addWidget(okButton, 2, 0);
        mainLayout->addWidget(cancelButton, 2, 1);

        // Set the size of the dialog box
        setFixedSize(400, 200);  // Set the desired width and height

    }  else if (whichPopUp == 1)                                                                //For adding a complete new node
    {
        setWindowTitle("Create New Node");

        // Create input fields
        addressSpinBox = new QSpinBox(this);
        elementSpinBox = new QSpinBox(this);
        nodeNameInput = new QLineEdit(this);
        sensorNameInput = new QLineEdit(this);
        actuatorNameInput = new QLineEdit(this);

        addressSpinBox->setRange(0, 1024);
        elementSpinBox->setRange(0, 1024);
        nodeNameInput->setMaxLength(12);
        sensorNameInput->setMaxLength(12);
        actuatorNameInput->setMaxLength(12);

        // Create buttons
        okButton = new QPushButton("OK", this);
        cancelButton = new QPushButton("Cancel", this);

        // Connect signals and slots for the buttons
        connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

        // Set up layout
        QGridLayout *mainLayout = new QGridLayout(this);
        mainLayout->addWidget(new QLabel("Node name:"), 0, 0);
        mainLayout->addWidget(nodeNameInput, 0, 1);
        mainLayout->addWidget(new QLabel("Node Address:"), 1, 0);
        mainLayout->addWidget(addressSpinBox, 1, 1);
        mainLayout->addWidget(new QLabel("Node Element:"), 2, 0);
        mainLayout->addWidget(elementSpinBox, 2, 1);
        mainLayout->addWidget(new QLabel("Sensor name:"), 3, 0);
        mainLayout->addWidget(sensorNameInput, 3, 1);
        mainLayout->addWidget(new QLabel("Actuator name:"), 4, 0);
        mainLayout->addWidget(actuatorNameInput, 4, 1);

        // Add buttons to the layout
        mainLayout->addWidget(okButton, 5, 0);
        mainLayout->addWidget(cancelButton, 5, 1);

        // Set the size of the dialog box
        setFixedSize(250, 300);  // Set the desired width and height
    }
}

//Returns node address from user input
int NodeDialogBox::getNodeAddress() const
{
    return addressSpinBox->value();
}

//Returns node element address from user input
int NodeDialogBox::getNodeElement() const
{
    return elementSpinBox->value();
}

//Returns the node name from user input
QString NodeDialogBox::getNodeName() const
{
    QString finalNodeName = nodeNameInput->text();
    for(int l = finalNodeName.length(); l < 12; l++)
    return finalNodeName;
}

//Returns the sensor name from user input
QString NodeDialogBox::getSensorName() const
{
    QString finalSensorName = sensorNameInput->text();
    return finalSensorName;
}

//Returns the actuator name from user input
QString NodeDialogBox::getActuatorName() const
{
    QString finalActuatorName = actuatorNameInput->text();
    return finalActuatorName;
}
