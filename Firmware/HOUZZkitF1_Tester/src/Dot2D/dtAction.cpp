#include "dtAction.h"
#include "dtActionInterval.h"
#include "dtNode.h"

NS_DT_BEGIN

inline float clampf(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive) {
        std::swap(min_inclusive, max_inclusive);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}

Action::Action()
:_originalTarget(nullptr)
,_target(nullptr)
,_tag(Action::INVALID_TAG)
,_flags(0)
{

}

Action::~Action()
{
    
}

void Action::startWithTarget(Node *aTarget)
{
    _originalTarget = _target = aTarget;
}

void Action::stop()
{
    _target = nullptr;
}

bool Action::isDone() const
{
    return true;
}

void Action::step(float /*dt*/)
{
    
}

void Action::update(float /*time*/)
{
    
}

//
// Speed
//
Speed::Speed()
: _speed(0.0)
, _innerAction(nullptr)
{
}

Speed::~Speed()
{
    DT_SAFE_RELEASE(_innerAction);
}

Speed* Speed::create(ActionInterval* action, float speed)
{
    Speed *ret = new Speed();
    if (ret && ret->initWithAction(action, speed))
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

bool Speed::initWithAction(ActionInterval *action, float speed)
{
    if (action == nullptr)
    {
        return false;
    }
    
    action->retain();
    _innerAction = action;
    _speed = speed;
    return true;
}

Speed *Speed::clone() const
{
    // no copy constructor
    if (_innerAction)
        return Speed::create(_innerAction->clone(), _speed);
    
    return nullptr;
}

void Speed::startWithTarget(Node* target)
{
    if (target && _innerAction)
    {
        Action::startWithTarget(target);
        _innerAction->startWithTarget(target);
    }
    else
    {

    }
}

void Speed::stop()
{
    if (_innerAction)
        _innerAction->stop();
    
    Action::stop();
}

void Speed::step(float dt)
{
    _innerAction->step(dt * _speed);
}

bool Speed::isDone() const
{
    return _innerAction->isDone();
}

Speed *Speed::reverse() const
{
    if (_innerAction)
        return Speed::create(_innerAction->reverse(), _speed);
    
    return nullptr;
}

void Speed::setInnerAction(ActionInterval *action)
{
    if (_innerAction != action)
    {
        DT_SAFE_RELEASE(_innerAction);
        _innerAction = action;
        DT_SAFE_RETAIN(_innerAction);
    }
}

//
// Follow
//
Follow::~Follow()
{
    DT_SAFE_RELEASE(_followedNode);
}

Follow* Follow::create(Node *followedNode, const Rect& rect/* = Rect::ZERO*/)
{
    return createWithOffset(followedNode, 0.0, 0.0,rect);
}

Follow* Follow::createWithOffset(Node* followedNode,float xOffset,float yOffset,const Rect& rect/*= Rect::ZERO*/){
    
    
    Follow *follow = new Follow();
    
    bool valid = false;
    
    if(follow)
        valid = follow->initWithTargetAndOffset(followedNode, xOffset, yOffset,rect);

    if (follow && valid)
    {
        follow->autorelease();
        return follow;
    }
    
    delete follow;
    return nullptr;
    
}
Follow* Follow::clone() const
{
    // no copy constructor
    return Follow::createWithOffset(_followedNode, _offsetX,_offsetY,_worldRect);
    
}

Follow* Follow::reverse() const
{
    return clone();
}

bool Follow::initWithTargetAndOffset(Node *followedNode, float xOffset,float yOffset,const Rect& rect)
{
    if(followedNode == nullptr)
    {
        return false;
    }
 
    followedNode->retain();
    _followedNode = followedNode;
    _worldRect = rect;
    _boundarySet = !rect.equals(Rect::ZERO);
    _boundaryFullyCovered = false;

    //Size winSize = Director::getInstance()->getWinSize();
    //_fullScreenSize.set(winSize.width, winSize.height); //TODO
    _halfScreenSize = _fullScreenSize * 0.5f;
    _offsetX=xOffset;
    _offsetY=yOffset;
    _halfScreenSize.x += _offsetX;
    _halfScreenSize.y += _offsetY;
    
    if (_boundarySet)
    {
        _leftBoundary = -((rect.origin.x+rect.size.width) - _fullScreenSize.x);
        _rightBoundary = -rect.origin.x ;
        _topBoundary = -rect.origin.y;
        _bottomBoundary = -((rect.origin.y+rect.size.height) - _fullScreenSize.y);

        if(_rightBoundary < _leftBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            _rightBoundary = _leftBoundary = (_leftBoundary + _rightBoundary) / 2;
        }
        if(_topBoundary < _bottomBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            _topBoundary = _bottomBoundary = (_topBoundary + _bottomBoundary) / 2;
        }

        if( (_topBoundary == _bottomBoundary) && (_leftBoundary == _rightBoundary) )
        {
            _boundaryFullyCovered = true;
        }
    }
    
    return true;
}

bool Follow::initWithTarget(Node *followedNode, const Rect& rect /*= Rect::ZERO*/){
    
    return initWithTargetAndOffset(followedNode, 0.0, 0.0,rect);
    
}
void Follow::step(float /*dt*/)
{
    if(_boundarySet)
    {
        // whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
        if(_boundaryFullyCovered)
        {
            return;
        }

        Vec2 tempPos = _halfScreenSize - _followedNode->getPosition();

        _target->setPosition(clampf(tempPos.x, _leftBoundary, _rightBoundary),
                                   clampf(tempPos.y, _bottomBoundary, _topBoundary));
    }
    else
    {
        _target->setPosition(_halfScreenSize - _followedNode->getPosition());
    }
}

bool Follow::isDone() const
{
    return ( !_followedNode->isRunning() );
}

void Follow::stop()
{
    _target = nullptr;
    Action::stop();
}

NS_DT_END
