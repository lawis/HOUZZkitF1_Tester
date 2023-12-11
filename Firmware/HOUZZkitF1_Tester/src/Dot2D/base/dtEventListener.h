#ifndef __DT_EVENT_LISTENER_H__
#define __DT_EVENT_LISTENER_H__

#include <functional>
#include <string>
#include <memory>

#include "dtMacros.h"
#include "dtRef.h"


/**
 * @addtogroup base
 * @{
 */

NS_DT_BEGIN

class Event;
class Node;

class EventListener : public Ref
{
public:

    enum class Type
    {
        BUTTON,
        PS2,
        SENSOR,
        CUSTOM
    };

    typedef std::string ListenerID;

protected:

    std::function<void(Event*)> _onEvent;

    Type _type;

    ListenerID _listenerID; 

    bool _isRegistered;

    int   _fixedPriority;

    Node* _node;

    bool _paused;

    bool _isEnabled;

    friend class EventDispatcher;

protected:

    EventListener();

    bool init(Type t, const ListenerID& listenerID, const std::function<void(Event*)>& callback);

    void setPaused(bool paused) { _paused = paused; }

    bool isPaused() const { return _paused; }

    void setRegistered(bool registered) { _isRegistered = registered; }

    bool isRegistered() const { return _isRegistered; }

    Type getType() const { return _type; }

    const ListenerID& getListenerID() const { return _listenerID; }

    void setFixedPriority(int fixedPriority) { _fixedPriority = fixedPriority; }

    int getFixedPriority() const { return _fixedPriority; }

    void setAssociatedNode(Node* node) { _node = node; }

    Node* getAssociatedNode() const { return _node; }

public:

    virtual ~EventListener();

    virtual bool checkAvailable() = 0;

    virtual EventListener* clone() = 0;

    void setEnabled(bool enabled) { _isEnabled = enabled; }

    bool isEnabled() const { return _isEnabled; }

};


NS_DT_END

// end of base group
/// @}



#endif //__DT_EVENT_LISTENER_H__
