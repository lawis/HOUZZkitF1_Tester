#ifndef __DT_EVENT_BUTTON_H__
#define __DT_EVENT_BUTTON_H__

#include "dtEvent.h"

NS_DT_BEGIN

class EventButton : public Event
{
public:

    enum class ButtonEventCode
    {
        BEC_NONE = 0,
        BEC_CLICK = 1,
        BEC_DOUBLE_CLICK,
        BEC_LONG_PRESS_START,
        BEC_LONG_PRESS_STOP,
        BEC_DURING_LONG_PRESS
    };

private:

    int8_t _btnCode;

    ButtonEventCode _btnEventCode;

protected:

    friend class EventListenerButton;
    
public:

    EventButton(int8_t btnCode, ButtonEventCode eventCode = ButtonEventCode::BEC_NONE);

    ~EventButton();

    int8_t btnCode(){return _btnCode;}

    ButtonEventCode eventCode(){return _btnEventCode;}
};



NS_DT_END


#endif //__DT_EVENT_BUTTON_H__