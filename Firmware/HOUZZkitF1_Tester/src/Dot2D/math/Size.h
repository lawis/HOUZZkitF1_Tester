#ifndef __SIZE_H__
#define __SIZE_H__

#include "../base/dtMacros.h"

NS_DT_BEGIN

/**
 * @addtogroup base
 * @{
 */

class Size
{
public:

    int32_t width;

    int32_t height;

public:

    INLINE Size()
    : width(0)
    , height(0)
    {

    };

    INLINE Size(int32_t width, int32_t height)
    : width(width)
    , height(height)
    {

    };

    INLINE Size(const Size& s)
    : width(s.width)
    , height(s.height)
    {

    };  

    INLINE void set(int32_t width, int32_t height)
    {
        this->width = width;
        this->height = height;
    };

    INLINE bool equals(const Size& s) const
    {
        return *this == s;
    };

    INLINE Size& operator= (const Size& s)
    {
        set(s.width, s.height);
        return *this;
    };

    INLINE Size operator+(const Size& s) const
    {
        return Size(this->width + s.width, this->height + s.height);
    };

    INLINE Size operator-(const Size& s) const
    {
        return Size(this->width - s.width, this->height - s.height);
    };

    INLINE Size operator*(float a) const
    {
        return Size(this->width * a, this->height * a);
    };

    INLINE Size operator/(float a) const
    {
        return Size(this->width / a, this->height / a);
    };

    INLINE bool operator==(const Size& s) const
    {
        return width == s.width && height == s.height;
    };

    INLINE bool operator!=(const Size& s) const
    {
        return width!=s.width || height!=s.height;
    };

public:

    static const Size ZERO;

};

/**
 end of base group
 @}
 */

NS_DT_END

#endif //__SIZE_H__