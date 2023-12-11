#include "dtRef.h"
#include "dtAutoreleasePool.h"

NS_DT_BEGIN

Ref::Ref()
:_referenceCount(1)
{

}

Ref::~Ref()
{

}

void Ref::retain()
{
    ++_referenceCount;
}

void Ref::release()
{
    --_referenceCount;
    if(_referenceCount == 0)
    {
        delete this;
    }
}

Ref* Ref::autorelease()
{
    AutoreleasePool::getInstance()->addObject(this);
    return this;
}

unsigned int Ref::getReferenceCount() const
{
    return _referenceCount;
}

NS_DT_END