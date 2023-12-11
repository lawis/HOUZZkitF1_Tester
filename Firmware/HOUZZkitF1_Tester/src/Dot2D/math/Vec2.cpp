#include "Vec2.h"
#include "dtMath.h"

NS_DT_BEGIN


void Vec2::add(const Vec2& v1, const Vec2& v2, Vec2* dst)
{
    dst->x = v1.x + v2.x;
    dst->y = v1.y + v2.y;
}

void Vec2::subtract(const Vec2& v1, const Vec2& v2, Vec2* dst)
{
    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
}

void Vec2::clamp(const Vec2& min, const Vec2& max)
{
    if (x < min.x)
        x = min.x;
    if (x > max.x)
        x = max.x;

    if (y < min.y)
        y = min.y;
    if (y > max.y)
        y = max.y;
}

void Vec2::clamp(const Vec2& v, const Vec2& min, const Vec2& max, Vec2* dst)
{
    dst->x = v.x;
    if (dst->x < min.x)
        dst->x = min.x;
    if (dst->x > max.x)
        dst->x = max.x;

    dst->y = v.y;
    if (dst->y < min.y)
        dst->y = min.y;
    if (dst->y > max.y)
        dst->y = max.y;
}

int32_t Vec2::dot(const Vec2& v1, const Vec2& v2)
{
    return (v1.x * v2.x + v1.y * v2.y);
}

void Vec2::normalize()
{
    float n = x * x + y * y;
    if (n == 1.0f)
        return;
    n = std::sqrt(n);
    if (n < MATH_TOLERANCE)
        return;
    n = 1.0f / n;
    x *= n;
    y *= n;
}

Vec2 Vec2::getNormalized() const
{
    Vec2 v(*this);
    v.normalize();
    return v;
}

void Vec2::rotate(const Vec2& point, float angle)
{
    float sinAngle = std::sin(angle);
    float cosAngle = std::cos(angle);

    if (point.isZero())
    {
        float tempX = x * cosAngle - y * sinAngle;
        y = y * cosAngle + x * sinAngle;
        x = tempX;
    }
    else
    {
        float tempX = x - point.x;
        float tempY = y - point.y;

        x = tempX * cosAngle - tempY * sinAngle + point.x;
        y = tempY * cosAngle + tempX * sinAngle + point.y;
    }
}

NS_DT_END