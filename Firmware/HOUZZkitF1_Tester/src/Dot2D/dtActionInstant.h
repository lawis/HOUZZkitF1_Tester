#ifndef __DT_INSTANT_ACTION_H__
#define __DT_INSTANT_ACTION_H__

#include <functional>
#include "dtAction.h"

NS_DT_BEGIN

/**
 * @addtogroup actions
 * @{
 */

class ActionInstant : public FiniteTimeAction
{
public:

    virtual ActionInstant* clone() const override
    {
        return nullptr;
    }
    
    virtual ActionInstant * reverse() const override
    {
        return nullptr;
    }

    virtual void startWithTarget(Node *target) override;
    
    virtual bool isDone() const override;

    virtual void step(float dt) override;

    virtual void update(float time) override;

    virtual int8_t actionClassType() override {return 1;}

private:

    bool _done;

};

class Show : public ActionInstant
{
public:

    static Show * create();

    virtual void update(float time) override;

    virtual ActionInstant* reverse() const override;

    virtual Show* clone() const override;

protected:

    Show(){}

    virtual ~Show(){}

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(Show);

};

class Hide : public ActionInstant
{
public:

    static Hide * create();

    virtual void update(float time) override;

    virtual ActionInstant* reverse() const override;

    virtual Hide* clone() const override;

protected:

    Hide(){}

    virtual ~Hide(){}

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(Hide);

};


class ToggleVisibility : public ActionInstant
{
public:

    static ToggleVisibility * create();

    virtual void update(float time) override;

    virtual ToggleVisibility* reverse() const override;

    virtual ToggleVisibility* clone() const override;

protected:

    ToggleVisibility(){}

    virtual ~ToggleVisibility(){}

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(ToggleVisibility);

};

class RemoveSelf : public ActionInstant
{
public:

    static RemoveSelf * create(bool isNeedCleanUp = true);

    virtual void update(float time) override;

    virtual RemoveSelf* clone() const override;

    virtual RemoveSelf* reverse() const override;
    
protected:

    RemoveSelf() : _isNeedCleanUp(true){}

    virtual ~RemoveSelf(){}

    bool init(bool isNeedCleanUp);

protected:

    bool _isNeedCleanUp;

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(RemoveSelf);

};

class FlipX : public ActionInstant
{
public:

    static FlipX * create(bool x);

    virtual void update(float time) override;

    virtual FlipX* reverse() const override;

    virtual FlipX* clone() const override;
    
protected:

    FlipX() :_flipX(false) {}

    virtual ~FlipX() {}

    bool initWithFlipX(bool x);

protected:

    bool    _flipX;

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(FlipX);

};

class FlipY : public ActionInstant
{
public:

    static FlipY * create(bool y);

    virtual void update(float time) override;

    virtual FlipY* reverse() const override;

    virtual FlipY* clone() const override;
    
protected:

    FlipY() :_flipY(false) {}

    virtual ~FlipY() {}

    bool initWithFlipY(bool y);

protected:
    bool    _flipY;

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(FlipY);

};

class  Place : public ActionInstant
{
public:

    static Place * create(const Vec2& pos);

    virtual void update(float time) override;

    virtual Place* reverse() const override;

    virtual Place* clone() const override;
    
protected:

    Place(){}

    virtual ~Place(){}

    bool initWithPosition(const Vec2& pos);

protected:

    Vec2 _position;

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(Place);

};

class CallFunc : public ActionInstant
{
public:

    static CallFunc * create(const std::function<void()>& func);

public:

    virtual void execute();

    virtual void update(float time) override;

    virtual CallFunc* reverse() const override;

    virtual CallFunc* clone() const override;
    
protected:

    CallFunc()
    : _callFunc(nullptr)
    , _function(nullptr)
    {
    }

    virtual ~CallFunc();

    bool initWithFunction(const std::function<void()>& func);

protected:

    union
    {
        SEL_CallFunc    _callFunc;
        SEL_CallFuncN    _callFuncN;
    };

    std::function<void()> _function;

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(CallFunc);

};

class CallFuncN : public CallFunc
{
public:

    static CallFuncN * create(const std::function<void(Node*)>& func);

    virtual CallFuncN* clone() const override;

    virtual void execute() override;
    
protected:

    CallFuncN():_functionN(nullptr){}

    virtual ~CallFuncN(){}

    bool initWithFunction(const std::function<void(Node*)>& func);
    
protected:
    
    std::function<void(Node*)> _functionN;

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(CallFuncN);

};

// end of actions group
/// @}

NS_DT_END


#endif //__DT_INSTANT_ACTION_H__