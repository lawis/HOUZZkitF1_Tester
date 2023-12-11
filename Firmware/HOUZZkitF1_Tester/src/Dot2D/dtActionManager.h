#ifndef __DT_ACTION_MANAGER_H__
#define __DT_ACTION_MANAGER_H__

#include "dtAction.h"
#include "base/dtVector.h"
#include "base/dtRef.h"

NS_DT_BEGIN

struct _hashElement;

class ActionManager : public Ref
{
public:

    ActionManager();

    ~ActionManager();

    virtual void addAction(Action *action, Node *target, bool paused);

    virtual void removeAllActions();

    virtual void removeAllActionsFromTarget(Node *target);

    virtual void removeAction(Action *action);

    virtual void removeActionByTag(int tag, Node *target);

    virtual void removeAllActionsByTag(int tag, Node *target);

    virtual void removeActionsByFlags(unsigned int flags, Node *target);

    virtual Action* getActionByTag(int tag, const Node *target) const;

    virtual ssize_t getNumberOfRunningActionsInTarget(const Node *target) const;
    
    virtual ssize_t getNumberOfRunningActions() const;

    virtual size_t getNumberOfRunningActionsInTargetByTag(const Node *target, int tag);

    virtual void pauseTarget(Node *target);

    virtual void resumeTarget(Node *target);
    
    virtual Vector<Node*> pauseAllRunningActions();
    
    virtual void resumeTargets(const Vector<Node*>& targetsToResume);
    
    virtual void update(float dt);
    
protected:

    void removeActionAtIndex(int32_t index, struct _hashElement *element);

    void deleteHashElement(struct _hashElement *element);

    void actionAllocWithHashElement(struct _hashElement *element);

protected:

    struct _hashElement    *_targets;

    struct _hashElement    *_currentTarget;

    bool            _currentTargetSalvaged;
};

NS_DT_END

#endif //__DT_ACTION_MANAGER_H__
