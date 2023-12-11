#ifndef __DT_EVENT_LISTENER_BUTTON_H__
#define __DT_EVENT_LISTENER_BUTTON_H__

#include "dtEventListener.h"
#include "dtEventButton.h"

NS_DT_BEGIN

class EventListenerButton : public EventListener
{
public:
    
    static const std::string LISTENER_ID;

    std::function<void(int8_t, Event*)> onBtnClick;

    std::function<void(int8_t, Event*)> onBtnDoubleClick;

    std::function<void(int8_t, Event*)> onBtnLongPressStart;

    std::function<void(int8_t, Event*)> onBtnLongPressStop;

    std::function<void(int8_t, Event*)> onBtnDuringLongPress;

protected:

    EventListenerButton(/* args */);

    ~EventListenerButton();

    bool init();

public:

    static EventListenerButton* create();

    /// Overrides
    virtual EventListenerButton* clone() override;

    virtual bool checkAvailable() override;
    
};




NS_DT_END



#endif //__DT_EVENT_LISTENER_BUTTON_H__