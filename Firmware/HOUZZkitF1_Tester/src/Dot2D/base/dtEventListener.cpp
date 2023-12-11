#include "dtEventListener.h"

NS_DT_BEGIN

EventListener::EventListener()
{

}
    
EventListener::~EventListener() 
{

}

bool EventListener::init(Type t, const ListenerID& listenerID, const std::function<void(Event*)>& callback)
{
    _onEvent = callback;
    _type = t;
    _listenerID = listenerID;
    _isRegistered = false;
    _paused = false;
    _isEnabled = true;
    return true;
}

bool EventListener::checkAvailable()
{ 
	return (_onEvent != nullptr);
}


NS_DT_END