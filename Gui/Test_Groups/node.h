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
    void addActuator(QString nodeName, QString actuatorType, QString actuatorName);

    QStringList getAllNodeNames() const;
    QString getSensors(QString nodeName, QString sensorType);
    QString getActuators(QString nodeName, QString actuatorType);

    struct NodeComponents
    {
        QMultiHash<QString, QString> sensorHash;
        QMultiHash<QString, QString> actuatorHash;
    };

private:

    //QMap with node name and node components
    QMap<QString, NodeComponents> nodeInstances;
};

#endif // NODE_H
