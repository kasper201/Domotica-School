// nodedialogbox.h

#ifndef NODEDIALOGBOX_H
#define NODEDIALOGBOX_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

class NodeDialogBox : public QDialog
{
    Q_OBJECT

public:
    NodeDialogBox(const QString& nodeName, int whichPopUp, QWidget *parent = nullptr);
    int getNodeAddress() const;
    int getNodeElement() const;
    QString getNodeName() const;
    QString getSensorName() const;
    QString getActuatorName() const;

private:
    QSpinBox *addressSpinBox;
    QSpinBox *elementSpinBox;
    QLineEdit *nodeNameInput;
    QLineEdit *sensorNameInput;
    QLineEdit *actuatorNameInput;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // NODEDIALOGBOX_H
