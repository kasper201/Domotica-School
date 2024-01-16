// nodedialogbox.cpp

#include "nodedialogbox.h"

NodeDialogBox::NodeDialogBox(const QString& nodeName, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Set Node properties for: " + nodeName);

    // Create input fields
    addressSpinBox = new QSpinBox(this);
    elementSpinBox = new QSpinBox(this);

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
}

int NodeDialogBox::getNodeAddress() const
{
    return addressSpinBox->value();
}

int NodeDialogBox::getNodeElement() const
{
    return elementSpinBox->value();
}
