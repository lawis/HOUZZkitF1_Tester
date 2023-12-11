#ifndef __DT_AUTORELEASEPOOL_H__
#define __DT_AUTORELEASEPOOL_H__

#include <vector>
#include <string>
#include "dtRef.h"

NS_DT_BEGIN

/**
 * @addtogroup base
 * @{
 */

class AutoreleasePool
{
public:

    static AutoreleasePool* getInstance();

    static void destroyInstance();

    void addObject(Ref *object);

    void clear();

    bool contains(Ref* object) const;

private:

    static AutoreleasePool* s_AutoreleasePool;

    std::vector<Ref*> _managedObjectArray;

    AutoreleasePool();

    ~AutoreleasePool();
    
};

// end of base group
/** @} */



NS_DT_END

#endif //__DT_AUTORELEASEPOOL_H__