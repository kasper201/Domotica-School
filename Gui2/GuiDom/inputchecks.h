#ifndef INPUTCHECKS_H
#define INPUTCHECKS_H
#include <QString>

class InputChecks
{
public:
    InputChecks();
    QString CheckForUuid(QString &input);
    QString CheckForStatus(QString &input);
};

#endif // INPUTCHECKS_H
