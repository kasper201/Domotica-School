#ifndef NODE_H
#define NODE_H

#include "actuator.h"
#include "sensor.h"
#include <QVector>

class Node
{
public:
    Node(QString, int);
    ~Node();
    QString getNodeName() const;
    int getNodeAddress();
    QStringList getNodeSensors() const;
    QStringList getNodeActuators() const;
    void addSensor(QString);
    void addActuator(QString);

private:
    QString nodeName;
    int nodeAddress;
    QVector<Sensor> sensors;
    QVector<Actuator> actuators;
};

#endif // NODE_H
