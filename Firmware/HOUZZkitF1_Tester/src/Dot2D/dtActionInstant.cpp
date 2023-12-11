#include "dtActionInstant.h"
#include "dtNode.h"
#include "dtSprite.h"

NS_DT_BEGIN

void ActionInstant::startWithTarget(Node *target)
{
    FiniteTimeAction::startWithTarget(target);
    _done = false;
}

bool ActionInstant::isDone() const
{
    return _done;
}

void ActionInstant::step(float /*dt*/)
{
    float updateDt = 1;
    update(updateDt);
}

void ActionInstant::update(float /*time*/)
{
    _done = true;
}

Show* Show::create() 
{
    Show* ret = new  Show();
    if (ret)
    {
        ret->autorelease();
    }
    return ret;
}

void Show::update(float time)
{
    ActionInstant::update(time);
    _target->setVisible(true);
}

ActionInstant* Show::reverse() const
{
    return Hide::create();
}

Show* Show::clone() const
{
    return Show::create();
}

Hide * Hide::create() 
{
    Hide *ret = new Hide();
    if (ret)
    {
        ret->autorelease();
    }
    return ret;
}

void Hide::update(float time)
{
    ActionInstant::update(time);
    _target->setVisible(false);
}

ActionInstant *Hide::reverse() const
{
    return Show::create();
}

Hide* Hide::clone() const
{
    return Hide::create();
}

ToggleVisibility * ToggleVisibility::create()
{
    ToggleVisibility *ret = new ToggleVisibility();
    if (ret)
    {
        ret->autorelease();
    }
    return ret;
}

void ToggleVisibility::update(float time)
{
    ActionInstant::update(time);
    _target->setVisible(!_target->isVisible());
}

ToggleVisibility * ToggleVisibility::reverse() const
{
    return ToggleVisibility::create();
}

ToggleVisibility * ToggleVisibility::clone() const
{
    return ToggleVisibility::create();
}

RemoveSelf * RemoveSelf::create(bool isNeedCleanUp /*= true*/) 
{
    RemoveSelf *ret = new RemoveSelf();
    if (ret && ret->init(isNeedCleanUp))
    {
        ret->autorelease();
    }
    return ret;
}

void RemoveSelf::update(float time)
{
    ActionInstant::update(time);
    _target->removeFromParentAndCleanup(_isNeedCleanUp);
}

RemoveSelf * RemoveSelf::clone() const
{
    return RemoveSelf::create(_isNeedCleanUp);
}

RemoveSelf *RemoveSelf::reverse() const
{
    return RemoveSelf::create(_isNeedCleanUp);
}

bool RemoveSelf::init(bool isNeedCleanUp)
{
    _isNeedCleanUp = isNeedCleanUp;
    return true;
}

FlipX *FlipX::create(bool x)
{
    FlipX *ret = new FlipX();
    if (ret && ret->initWithFlipX(x))
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

void FlipX::update(float time)
{
    ActionInstant::update(time);
    static_cast<Sprite*>(_target)->setFlippedX(_flipX);
}

FlipX* FlipX::reverse() const
{
    return FlipX::create(!_flipX);
}

FlipX * FlipX::clone() const
{
    return FlipX::create(_flipX);
}

bool FlipX::initWithFlipX(bool x)
{
    _flipX = x;
    return true;
}

FlipY * FlipY::create(bool y)
{
    FlipY *ret = new FlipY();
    if (ret && ret->initWithFlipY(y))
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

void FlipY::update(float time)
{
    ActionInstant::update(time);
    static_cast<Sprite*>(_target)->setFlippedY(_flipY);
}

FlipY* FlipY::reverse() const
{
    return FlipY::create(!_flipY);
}

FlipY * FlipY::clone() const
{
    return FlipY::create(_flipY);
}

bool FlipY::initWithFlipY(bool y)
{
    _flipY = y;
    return true;
}

Place* Place::create(const Vec2& pos)
{
    Place *ret = new Place();
    if (ret && ret->initWithPosition(pos))
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}



Place * Place::clone() const
{
    // no copy constructor
    return Place::create(_position);
}

Place * Place::reverse() const
{
    // no reverse, just clone
    return this->clone();
}

void Place::update(float time)
{
    ActionInstant::update(time);
    _target->setPosition(_position); 
}

bool Place::initWithPosition(const Vec2& pos)
{
    _position = pos;
    return true;
}

CallFunc * CallFunc::create(const std::function<void()> &func)
{
    CallFunc *ret = new CallFunc();
    if (ret && ret->initWithFunction(func) )
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

bool CallFunc::initWithFunction(const std::function<void()> &func)
{
    _function = func;
    return true;
}

CallFunc::~CallFunc()
{

}

CallFunc * CallFunc::clone() const
{
    auto a = new CallFunc();
    if( _function )
    {
        a->initWithFunction(_function);
    }
    a->autorelease();
    return a;
}

CallFunc * CallFunc::reverse() const
{
    return this->clone();
}

void CallFunc::update(float time)
{
    ActionInstant::update(time);
    this->execute();
}

void CallFunc::execute()
{
    if( _function )
    {
        _function();
    }
}


CallFuncN * CallFuncN::create(const std::function<void(Node*)> &func)
{
    auto ret = new CallFuncN();
    if (ret && ret->initWithFunction(func) )
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

void CallFuncN::execute()
{
    if (_functionN)
    {
        _functionN(_target);
    }
}

bool CallFuncN::initWithFunction(const std::function<void (Node *)> &func)
{
    _functionN = func;
    return true;
}

CallFuncN * CallFuncN::clone() const
{
    auto a = new CallFuncN();
    if( _functionN ){
        a->initWithFunction(_functionN);
    }
    a->autorelease();
    return a;
}

NS_DT_END