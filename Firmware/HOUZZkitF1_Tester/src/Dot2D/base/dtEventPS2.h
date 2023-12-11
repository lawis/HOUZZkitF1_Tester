#ifndef __DT_EVENT_PS2_H__
#define __DT_EVENT_PS2_H__

#include "dtEvent.h"

NS_DT_BEGIN

class EventPS2 : public Event
{
public:

    enum class PS2EventCode
    {
        PEC_NONE = 0,
        PEC_CLICK = 1,
        PEC_JOYSTICK,
    };

private:

    uint16_t _btnCode;

    uint8_t _btnAnalog;

    PS2EventCode _ps2EventCode;

protected:

    friend class EventListenerButton;
    
public:

    EventPS2(uint16_t btnCode, uint8_t btnAnalog, PS2EventCode eventCode = PS2EventCode::PEC_NONE);

    ~EventPS2();

    uint16_t btnCode(){return _btnCode;}

    uint8_t btnAnalog(){return _btnAnalog;}

    PS2EventCode eventCode(){return _ps2EventCode;}
};



NS_DT_END


#endif //__DT_EVENT_PS2_H__
