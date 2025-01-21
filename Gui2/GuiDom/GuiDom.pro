QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    actuator.cpp \
    comport.cpp \
    groups.cpp \
    inputchecks.cpp \
    main.cpp \
    domotica.cpp \
    node.cpp \
    nodes.cpp \
    sensor.cpp

HEADERS += \
    actuator.h \
    comport.h \
    domotica.h \
    easystring.h \
    groups.h \
    inputchecks.h \
    node.h \
    nodes.h \
    sensor.h

FORMS += \
    domotica.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
