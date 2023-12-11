#include "dtTransform.h"

NS_DT_BEGIN

Transform::Transform(const Vec2& translation)
:_translation(translation)
{

}

Transform::Transform(const Transform& transform)
:_translation(transform._translation)
{
    
}
    
Transform::~Transform()
{

}

void Transform::setTranslation(const Vec2& translation)
{
    _translation = translation;
}

Transform Transform::transform(const Transform& parentTransform) const
{
    Transform res = Transform(*this);
    res._translation += parentTransform._translation;
    return res;
}

void Transform::transform(int32_t *x,int32_t *y) const
{
    *x += _translation.x;
    *y += _translation.y;
}

const Transform Transform::IDENTITY = Transform(Vec2(0,0));


NS_DT_END