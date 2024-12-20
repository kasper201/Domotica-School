#ifndef NODES_H
#define NODES_H

#include "domotica.h"
#include "node.h"
#include <QObject>

class Nodes : public QObject
{
    Q_OBJECT
public:
    explicit Nodes(Domotica*, QObject* parent = nullptr);
    ~Nodes();
    void addNode(QString nodeName);
    void addSensorToNode(QString nodeName, QString sensorName);
    void addActuatorToNode(QString nodeName, QString actuatorName);
    QStringList getNodeNames();

public slots:
    void showParts(QString nodeName);

private:
    Domotica* UIdomotica;
    QVector<Node> nodes;
};

#endif // NODES_H
