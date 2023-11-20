#include "sensor.h"

Sensor::Sensor()
{

}

Sensor::~Sensor()
{

}

//Triggers when a sensor sends a signal
QStringList Sensor::sensorTrigger(Groups& groups, QString data)
{
    QStringList groupsTriggered;
    QString sensorName;
    QString nodeName;
    while(data.contains("TriggerSensor")) //checks if there is sensor trigger and finds the belonging groups
    {
        data = stringM.removedTillWhitespace(data);
        nodeName = stringM.removedFromWhitespace(data);
        data = stringM.removedTillWhitespace(data);
        sensorName = stringM.removedFromWhitespace(data);
        data = stringM.removedTillWhitespace(data);

    }
    return groups.checkGroups(sensorName, nodeName); //Shows which groups are triggered
}

QStringList Sensor::groupTriggered(Groups& groups, Node& node, QString groupName)
{
    QStringList actuatorsTriggered = groups.getActuators(groupName);
    AllUpdates.clear();
    for (const QString& groupName : actuatorsTriggered)
    {
        QString actuatorName = groupName.split('\t').value(0);
        QString actuatorNode = groupName.split('\t').value(1);
        QString actuatorStatus = node.getActuatorStatus(actuatorNode, actuatorName);
        QString actuatorStatusInverted;
        if(actuatorStatus == "false")
        {
            actuatorStatusInverted = "true";
        } else if(actuatorStatus == "true")
        {
            actuatorStatusInverted = "false";
        }
        QString updateActuator = "UpdateActuator " + actuatorNode + " " + actuatorName + " " + actuatorStatusInverted;
        AllUpdates.append(updateActuator);
        qDebug() << "Update for actuator: " << actuatorName;
    }
    return AllUpdates;
}

void Sensor::actuatorUpdate(Node& node, QString data)
{
    while(data.contains("UpdateAppActuator"))
    {
        data = stringM.removedTillWhitespace(data);
        QString nodeName = stringM.removedFromWhitespace(data);
        data = stringM.removedTillWhitespace(data);
        QString actuatorName = stringM.removedFromWhitespace(data);
        data = stringM.removedTillWhitespace(data);
        QString actuatorStatus = stringM.removedFromWhitespace(data);
        data = stringM.removedTillWhitespace(data);
        node.updateActuatorStatus(nodeName, actuatorName, actuatorStatus);
        qDebug() << "updated actuator status: " << actuatorStatus;
    }
}
