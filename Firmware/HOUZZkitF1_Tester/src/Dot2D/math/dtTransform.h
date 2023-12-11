#ifndef __DT_TRANSFORM_H__
#define __DT_TRANSFORM_H__

#include "../base/dtMacros.h"
#include "Vec2.h"

NS_DT_BEGIN

/**
 * @addtogroup base
 * @{
 */

class Transform
{
protected:

    Vec2 _translation;

    // float rotate;

    // float scale;

public:

    Transform(const Vec2& translation);

    Transform(const Transform& transform);
    
    ~Transform();

    void setTranslation(const Vec2& translation);

    Transform transform(const Transform& parentTransform) const;

    void transform(int32_t *x,int32_t *y) const;

    static const Transform IDENTITY;
};




/**
 end of base group
 @}
 */

NS_DT_END

#endif //__DT_TRANSFORM_H__