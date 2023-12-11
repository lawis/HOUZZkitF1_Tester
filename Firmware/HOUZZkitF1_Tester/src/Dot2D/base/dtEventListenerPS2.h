#ifndef __DT_EVENT_LISTENER_PS2_H__
#define __DT_EVENT_LISTENER_PS2_H__

#include "dtEventListener.h"
#include "dtEventPS2.h"

NS_DT_BEGIN

class EventListenerPS2 : public EventListener
{
public:
    
    static const std::string LISTENER_ID;

    std::function<void(uint16_t, uint8_t, Event*)> onPs2Click;

    std::function<void(uint16_t, uint8_t, Event*)> onPs2Joystick;

protected:

    EventListenerPS2();

    ~EventListenerPS2();

    bool init();

public:

    static EventListenerPS2* create();

    /// Overrides
    virtual EventListenerPS2* clone() override;

    virtual bool checkAvailable() override;
    
};




NS_DT_END



#endif //__DT_EVENT_LISTENER_PS2_H__
