#include "dtEventPS2.h"

NS_DT_BEGIN

EventPS2::EventPS2(uint16_t btnCode,uint8_t btnAnalog, PS2EventCode eventCode)
: Event(Type::PS2)
, _btnCode(btnCode)
, _btnAnalog(btnAnalog)
, _ps2EventCode(eventCode)
{

}

 EventPS2::~EventPS2()
 {

 }

NS_DT_END
