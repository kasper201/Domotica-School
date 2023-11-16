#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QMultiHash>
#include <map>
#include <QDebug>

class Node
{
public:
    Node();
    ~Node();

    void addNodeInstance(QString nodeName);
    void addSensor(QString nodeName, QString sensorType, QString sensorName);
    void addActuator(QString nodeName, QString actuatorType, QString actuatorName, QString actuatorStatus);

    QStringList getAllNodeNames() const;
    QStringList getAllSensorNames(QString nodeName) const;
    QStringList getAllActuatorNames(QString nodeName) const;
    bool getActuatorStatus(QString nodeName, QString actuatorName);

    struct NodeComponents
    {
        QMultiHash<QString, QString> sensorHash;
        QMultiHash<QString, QString> actuatorHash;
        QMultiHash<QString, bool> actuatorState;
    };

private:

    //QMap with node name and node components
    QMap<QString, NodeComponents> nodeInstances;
};

#endif // NODE_H
