#ifndef LEDTOGGLE_H
#define LEDTOGGLE_H

#include <QMessageBox>
#include <QString>
#include <QLabel>
#include <iostream>

class LedToggle
{
public:
    LedToggle();
    QString sendLedToggle(QString led, bool onOff);
};

#endif // LEDTOGGLE_H
