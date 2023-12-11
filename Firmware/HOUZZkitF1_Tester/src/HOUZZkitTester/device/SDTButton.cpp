#include "SDTButton.h"

SDTButton::SDTButton(const int pin, const boolean activeLow, const bool pullupActive)
:OneButton(pin,activeLow,pullupActive)
{

}

SDTButton::~SDTButton()
{

}