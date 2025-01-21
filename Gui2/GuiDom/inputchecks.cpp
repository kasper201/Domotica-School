#include "inputchecks.h"
#include <QRegularExpression>

InputChecks::InputChecks()
{
}

QString InputChecks::CheckForUuid(QString &input)
{
    // Define the pattern to look for the UUID followed by 16 zeros and a comma
    static const QRegularExpression regex(R"(UUID ([0-9a-fA-F]{16})0000000000000000,)");
    QRegularExpressionMatch match = regex.match(input);

    if (match.hasMatch()) {
        // Capture group 1 contains the 16-character UUID
        return match.captured(1);
    } else {
        // Return an empty string if the pattern is not found
        return QString();
    }
}
