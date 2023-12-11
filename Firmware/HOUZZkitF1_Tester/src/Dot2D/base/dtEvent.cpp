#include "dtEvent.h"

NS_DT_BEGIN

Event::Event(Type type)
: _type(type)
, _isStopped(false)
, _currentTarget(nullptr)
{

}

Event::~Event()
{
    
}

NS_DT_END