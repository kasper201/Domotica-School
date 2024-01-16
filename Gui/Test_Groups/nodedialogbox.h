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
    NodeDialogBox(const QString& nodeName, QWidget *parent = nullptr);
    int getNodeAddress() const;
    int getNodeElement() const;

private:
    QSpinBox *addressSpinBox;
    QSpinBox *elementSpinBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // NODEDIALOGBOX_H
