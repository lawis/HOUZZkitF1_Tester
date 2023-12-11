#ifndef __DT_EVENT_H__
#define __DT_EVENT_H__

#include "dtMacros.h"
#include "dtRef.h"

/**
 * @addtogroup base
 * @{
 */

NS_DT_BEGIN

class Node;

class Event :public Ref
{
public:

    enum class Type
    {
        BUTTON,
        PS2,
        SENSOR,
        CUSTOM
    };

protected:

    Type _type; 

    bool _isStopped;

    Node* _currentTarget;

    friend class EventDispatcher;

private:
    


protected:

    Event(Type type);

    void setCurrentTarget(Node* target) { _currentTarget = target; }

public:

    virtual ~Event();

    Type getType() const { return _type; }

    void stopPropagation() { _isStopped = true; }

    bool isStopped() const { return _isStopped; }

    Node* getCurrentTarget() { return _currentTarget; }

};




NS_DT_END

// end of base group
/// @}


#endif //__DT_EVENT_H__
