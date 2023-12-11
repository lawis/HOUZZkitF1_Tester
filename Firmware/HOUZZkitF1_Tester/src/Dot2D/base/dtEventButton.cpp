#include "dtEventButton.h"

NS_DT_BEGIN

EventButton::EventButton(int8_t btnCode, ButtonEventCode eventCode)
: Event(Type::BUTTON)
, _btnCode(btnCode)
, _btnEventCode(eventCode)
{

}

 EventButton::~EventButton()
 {

 }

NS_DT_END