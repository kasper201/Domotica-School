#ifndef NODE_H
#define NODE_H

#include "actuator.h"
#include "sensor.h"
#include <QVector>

class Node
{
public:
    Node(QString);
    ~Node();
    QString getNodeName() const;
    QStringList getNodeSensors() const;
    QStringList getNodeActuators() const;
    void addSensor(QString);
    void addActuator(QString);

private:
    QString nodeName;
    QVector<Sensor> sensors;
    QVector<Actuator> actuators;
};

#endif // NODE_H
