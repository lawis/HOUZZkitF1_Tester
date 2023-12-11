#include "dtAutoreleasePool.h"
#include "dtMacros.h"

NS_DT_BEGIN

AutoreleasePool* AutoreleasePool::s_AutoreleasePool = nullptr;

AutoreleasePool* AutoreleasePool::getInstance()
{
    if (s_AutoreleasePool == nullptr)
    {
        s_AutoreleasePool = new AutoreleasePool();
    }
    return s_AutoreleasePool;
}

void AutoreleasePool::destroyInstance()
{
    delete s_AutoreleasePool;
    s_AutoreleasePool = nullptr;
}

void AutoreleasePool::addObject(Ref* object)
{
    _managedObjectArray.push_back(object);
}

void AutoreleasePool::clear()
{
    std::vector<Ref*> releasings;
    releasings.swap(_managedObjectArray);
    for (const auto &obj : releasings)
    {
        obj->release();
    }
}

bool AutoreleasePool::contains(Ref* object) const
{
    for (const auto& obj : _managedObjectArray)
    {
        if (obj == object)
            return true;
    }
    return false;
}

AutoreleasePool::AutoreleasePool()
{
    _managedObjectArray.reserve(50);
}

AutoreleasePool::~AutoreleasePool()
{
    clear();
}

NS_DT_END