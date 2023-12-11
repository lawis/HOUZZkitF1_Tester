#include "dtEventListenerPS2.h"

NS_DT_BEGIN

const std::string EventListenerPS2::LISTENER_ID = "__dt_ps2";

EventListenerPS2::EventListenerPS2()
: onPs2Click(nullptr)
, onPs2Joystick(nullptr)
{

}

EventListenerPS2::~EventListenerPS2()
{

}

bool EventListenerPS2::init()
{
    auto listener = [this](Event* event){
        auto ps2Event = static_cast<EventPS2*>(event);
        switch (ps2Event->eventCode())
        {
        case EventPS2::PS2EventCode::PEC_CLICK:
        {
            if(onPs2Click != nullptr)
            {
                onPs2Click(ps2Event->btnCode(),ps2Event->btnAnalog(),event);
            }
        }
            break;
        case EventPS2::PS2EventCode::PEC_JOYSTICK:
        {
            if(onPs2Joystick!= nullptr)
            {
                onPs2Joystick(ps2Event->btnCode(),ps2Event->btnAnalog(),event);
            }
        }
            break;        
        default:
            break;
        }
    };
    
    if (EventListener::init(Type::PS2, LISTENER_ID, listener))
    {
        return true;
    }
    
    return false;
}

EventListenerPS2* EventListenerPS2::create()
{
    auto ret = new EventListenerPS2();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        DT_SAFE_DELETE(ret);
    }
    return ret;
}

EventListenerPS2* EventListenerPS2::clone()
{
    auto ret = new EventListenerPS2();
    if (ret && ret->init())
    {
        ret->autorelease();
        ret->onPs2Click = onPs2Click;
        ret->onPs2Joystick = onPs2Joystick;
    }
    else
    {
        DT_SAFE_DELETE(ret);
    }
    return ret;
}

bool EventListenerPS2::checkAvailable()
{
    if (onPs2Click == nullptr && 
    onPs2Joystick == nullptr)
    {
        return false;
    }
    return true;
}

NS_DT_END
