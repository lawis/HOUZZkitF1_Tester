#ifndef __RECT_H__
#define __RECT_H__

#include "../base/dtMacros.h"
#include "Vec2.h"
#include "Size.h"

NS_DT_BEGIN

/**
 * @addtogroup base
 * @{
 */

class Rect
{
public:

    Vec2 origin;

    Size size;

public:

    Rect();

    Rect(int32_t x, int32_t y, int32_t width, int32_t height);

    Rect(const Vec2& v, const Size& s);

    Rect(const Rect& r);

    void set(int32_t x, int32_t y, int32_t width, int32_t height);

    bool equals(const Rect& r) const;
    
    Rect& operator= (const Rect& r);

    bool operator==(const Rect& r) const;

    bool operator!=(const Rect& r) const;

    float getMinX() const;

    float getMidX() const;

    float getMaxX() const;

    float getMinY() const;

    float getMidY() const;

    float getMaxY() const;

    bool containsPoint(const Vec2& point) const;

    bool intersectsRect(const Rect& rect) const;

    bool intersectsCircle(const Vec2& center, float radius) const;

    Rect unionWithRect(const Rect & rect) const;
    
    void merge(const Rect& rect);

public:
    
    static const Rect ZERO;

};

/**
 end of base group
 @}
 */

NS_DT_END

#endif //__RECT_H__