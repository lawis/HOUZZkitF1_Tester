#ifndef __DT_REF_H__
#define __DT_REF_H__

#include "dtMacros.h"

/**
 * @addtogroup base
 * @{
 */

NS_DT_BEGIN

class Ref;

class  Clonable
{
public:

    virtual Clonable* clone() const = 0;

    virtual ~Clonable() {};

};

class Ref
{
public:

    void retain();

    void release();

    Ref* autorelease();

    unsigned int getReferenceCount() const;

    virtual int8_t refClassType(){return 0;}

protected:

    Ref();

public:

    virtual ~Ref();

protected:

    unsigned int _referenceCount;

    friend class AutoreleasePool;

};

class Node;

typedef void (Ref::*SEL_CallFunc)();
typedef void (Ref::*SEL_CallFuncN)(Node*);
typedef void (Ref::*SEL_CallFuncND)(Node*, void*);
typedef void (Ref::*SEL_CallFuncO)(Ref*);
typedef void (Ref::*SEL_MenuHandler)(Ref*);
typedef void (Ref::*SEL_SCHEDULE)(float);

#define DT_CALLFUNC_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_CallFunc>(&_SELECTOR)
#define DT_CALLFUNCN_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_CallFuncN>(&_SELECTOR)
#define DT_CALLFUNCND_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_CallFuncND>(&_SELECTOR)
#define DT_CALLFUNCO_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_CallFuncO>(&_SELECTOR)
#define DT_MENU_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_MenuHandler>(&_SELECTOR)
#define DT_SCHEDULE_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_SCHEDULE>(&_SELECTOR)

NS_DT_END

// end of base group
/** @} */

#endif //__DT_REF_H__