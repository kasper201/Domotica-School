#include "ledtoggle.h"

LedToggle::LedToggle()
{

}

QString LedToggle::sendLedToggle(QString led, bool onOff)
{
    if (led == "Yellow")
    {
        if(onOff)
        {
            return "Yellow_On";
        } else
        {
            return "Yellow_Off";
        }
    } else if (led == "Blue")
    {
        if(onOff)
        {
            return "Blue_On";
        } else
        {
            return "Blue_Off";
        }
    } else if (led == "Red")
    {
        if(onOff)
        {
            return "Red_On";
        } else
        {
            return "Red_Off";
        }
    } else if (led == "Green")
    {
        if(onOff)
        {
            return "Green_On";
        } else
        {
            return "Green_Off";
        }
    } else {
        std::cerr << "Invalid color string!" << std::endl;
        return "error";
    }
}
