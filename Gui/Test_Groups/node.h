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

    void setNodeName(QString nodeName);
    void addSensor(QString sensorType, QString sensorName);
    void addActuator(QString actuatorType, QString actuatorName);

    QString getNodeName();
    QString getSensors(QString sensorType);
    QString getActuators(QString actuatorType);

private:
    QString name;

    struct nodeComponents
    {
        QMultiHash<QString, QString> sensorHash;
        QMultiHash<QString, QString> actuatorHash;
    } components;
    //zet ze in een struct en zoek het dan uit
};

#endif // NODE_H
