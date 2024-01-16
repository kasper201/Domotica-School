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

    void addNodeInstance(QString nodeName, int nodeAddress, int nodeElement);
    void addSensor(QString nodeName, QString sensorType, QString sensorName);
    void addActuator(QString nodeName, QString actuatorType, QString actuatorName, QString actuatorStatus);

    QStringList getAllNodeNames() const;
    QStringList getAllSensorNames(QString nodeName) const;
    QStringList getAllActuatorNames(QString nodeName) const;
    QString getActuatorStatus(QString nodeName, QString actuatorName);
    void updateActuatorStatus(QString nodeName, QString actuatorName, QString actuatorStatus);

    int getNodeAddress(QString nodeName) const;
    int getNodeElement(QString nodeName) const;

    struct NodeComponents
    {
        QMultiHash<QString, QString> sensorHash;
        QMultiHash<QString, QString> actuatorHash;
        QMultiHash<QString, QString> actuatorState;
        int nodeElement;
    };

private:

    //QMap with node name and node components
    QMap<QString, NodeComponents> nodeInstances;

    //QMap with node name and the node address
    QMap<QString, int> nodeAddress;
};

#endif // NODE_H
