#include "sensor.h"

Sensor::Sensor()
{

}

Sensor::~Sensor()
{

}

//Triggers when a sensor sends a signal
void Sensor::sensorTrigger(Groups& groups, Node& node, PortSetup& comport, QString data)
{
    while(data.contains("TriggerSensor")) //checks if there is sensor trigger
    {
        data = stringM.removedTillWhitespace(data);
        QString nodeName = stringM.removedFromWhitespace(data);
        data = stringM.removedTillWhitespace(data);
        QString sensorName = stringM.removedFromWhitespace(data);
        data = stringM.removedTillWhitespace(data);

        QStringList groupsTriggered = groups.checkGroups(sensorName, nodeName);
        for (const QString& groupName : groupsTriggered)
        {
            groupTriggered(groups, node, comport, groupName);
        }
    }
}

void Sensor::groupTriggered(Groups& groups, Node& node, PortSetup& comport, QString groupName)
{
    QStringList actuatorsTriggered = groups.getActuators(groupName);
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
        comport.WriteToComport(updateActuator);

    }
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
    }
}
