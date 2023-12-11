#ifndef __DT_EVENT_DISPATCHER_H__
#define __DT_EVENT_DISPATCHER_H__

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>

#include "dtMacros.h"
#include "dtEventListener.h"
#include "dtEvent.h"


/**
 * @addtogroup base
 * @{
 */

NS_DT_BEGIN

class Event;
class Node;

class EventDispatcher : public Ref
{
public:

    void addEventListenerWithSceneGraphPriority(EventListener* listener, Node* node);

    void addEventListenerWithFixedPriority(EventListener* listener, int fixedPriority);

    // EventListenerCustom* addCustomEventListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback);

    void removeEventListener(EventListener* listener);

    void removeEventListenersForType(EventListener::Type listenerType);

    void removeEventListenersForTarget(Node* target, bool recursive = false);
    
    // void removeCustomEventListeners(const std::string& customEventName);

    void removeAllEventListeners();

    void pauseEventListenersForTarget(Node* target, bool recursive = false);
    
    void resumeEventListenersForTarget(Node* target, bool recursive = false);
    
    void setPriority(EventListener* listener, int fixedPriority);

    void setEnabled(bool isEnabled);

    bool isEnabled() const;

    void dispatchEvent(Event* event);

    void dispatchCustomEvent(const std::string &eventName, void *optionalUserData = nullptr);

    bool hasEventListener(const EventListener::ListenerID& listenerID) const;

    EventDispatcher();

    ~EventDispatcher();

protected:

    friend class Node;
    
    void setDirtyForNode(Node* node);

    class EventListenerVector
    {
    public:

        EventListenerVector();

        ~EventListenerVector();

        size_t size() const;

        bool empty() const;
        
        void push_back(EventListener* item);

        void clearSceneGraphListeners();

        void clearFixedListeners();

        void clear();
        
        std::vector<EventListener*>* getFixedPriorityListeners() const { return _fixedListeners; }
        
        std::vector<EventListener*>* getSceneGraphPriorityListeners() const { return _sceneGraphListeners; }
        
        ssize_t getGt0Index() const { return _gt0Index; }
        
        void setGt0Index(ssize_t index) { _gt0Index = index; }

    private:

        std::vector<EventListener*>* _fixedListeners;

        std::vector<EventListener*>* _sceneGraphListeners;

        ssize_t _gt0Index;

    };
    
    void addEventListener(EventListener* listener);
    
    void forceAddEventListener(EventListener* listener);

    EventListenerVector* getListeners(const EventListener::ListenerID& listenerID) const;

    void updateDirtyFlagForSceneGraph();
    
    void removeEventListenersForListenerID(const EventListener::ListenerID& listenerID);
    
    void sortEventListeners(const EventListener::ListenerID& listenerID);
    
    void sortEventListenersOfSceneGraphPriority(const EventListener::ListenerID& listenerID, Node* rootNode);
    
    void sortEventListenersOfFixedPriority(const EventListener::ListenerID& listenerID);
    
    void updateListeners(Event* event);

    void associateNodeAndEventListener(Node* node, EventListener* listener);
    
    void dissociateNodeAndEventListener(Node* node, EventListener* listener);
    
    void dispatchEventToListeners(EventListenerVector* listeners, const std::function<bool(EventListener*)>& onEvent);
    
    void dispatchTouchEventToListeners(EventListenerVector* listeners, const std::function<bool(EventListener*)>& onEvent);
    
    void releaseListener(EventListener* listener);
    
    enum class DirtyFlag
    {
        NONE = 0,
        FIXED_PRIORITY = 1 << 0,
        SCENE_GRAPH_PRIORITY = 1 << 1,
        ALL = FIXED_PRIORITY | SCENE_GRAPH_PRIORITY
    };
    
    void setDirty(const EventListener::ListenerID& listenerID, DirtyFlag flag);
    
    void visitTarget(Node* node, bool isRootNode);

    void cleanToRemovedListeners();

    std::unordered_map<EventListener::ListenerID, EventListenerVector*> _listenerMap;
    
    std::unordered_map<EventListener::ListenerID, DirtyFlag> _priorityDirtyFlagMap;
    
    std::unordered_map<Node*, std::vector<EventListener*>*> _nodeListenersMap;
    
    std::unordered_map<Node*, int> _nodePriorityMap;
    
    std::unordered_map<float, std::vector<Node*>> _globalZOrderNodeMap;
    
    std::vector<EventListener*> _toAddedListeners;

    std::vector<EventListener*> _toRemovedListeners;

    std::set<Node*> _dirtyNodes;
    
    int _inDispatch;
    
    bool _isEnabled;
    
    int _nodePriorityIndex;
    
    std::set<std::string> _internalCustomListenerIDs;
};



NS_DT_END

// end of base group
/// @}

#endif //__DT_EVENT_DISPATCHER_H__
