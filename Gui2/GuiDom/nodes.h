#ifndef NODES_H
#define NODES_H

#include "domotica.h"
#include "node.h"
#include "easystring.h"
#include <QObject>

class Nodes : public QObject
{
    Q_OBJECT
public:
    explicit Nodes(Domotica*, QObject* parent = nullptr);
    ~Nodes();
    void addNode(QString nodeName, int nodeAddress);
    void addSensorToNode(QString nodeName, QString sensorName);
    void addActuatorToNode(QString nodeName, QString actuatorName);
    int FirstFreeNodeAddress();
    QStringList getNodeNames();
    int getNodeAddress(QString nodeName);

public slots:
    void showParts(QString nodeName);
    void removeNode(QString nodeName);

private:
    Domotica* UIdomotica;
    QVector<Node> nodes;
};

#endif // NODES_H
