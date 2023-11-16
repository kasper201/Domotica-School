#include "stringmodifiers.h"

StringModifiers::StringModifiers()
{

}

StringModifiers::~StringModifiers()
{

}

QString StringModifiers::removedTillWhitespace(QString string)
{
    int firstWhitespaceIndex; //where the first whitespace of a string is located
    firstWhitespaceIndex = string.indexOf(' ');

    // Check if a whitespace was found
    if (firstWhitespaceIndex != -1) {
        // Remove the substring from the beginning of the string up to the first whitespace
        string.remove(0, firstWhitespaceIndex + 1);
    }
    return string;
}

QString StringModifiers::removedFromWhitespace(QString string)
{
    int firstWhitespaceIndex; //where the first whitespace of a string is located
    firstWhitespaceIndex = string.indexOf(' ');

    // Check if a whitespace was found
    if (firstWhitespaceIndex != -1) {
        // Remove the substring from the beginning of the string up to the first whitespace
        string.remove(firstWhitespaceIndex, string.length());
    }
    return string;
}
