#ifndef STRINGMODIFIERS_H
#define STRINGMODIFIERS_H

#include <QDebug>
#include <QString>

class StringModifiers
{
public:
    StringModifiers();
    ~StringModifiers();

    QString removedTillWhitespace(QString string);
    QString removedFromWhitespace(QString string);
};

#endif // STRINGMODIFIERS_H
