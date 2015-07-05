#include "stdafx.h"
#include "GPTransform.h"
#include "gp.h"

namespace GPEngine3D {
    Transform::Transform(void):
    _transform()
    {
        
    }
    
    Transform::Transform(Matrix4 &m)
    {
        _transform = m;
    }
    
    Transform::~Transform(void)
    {
        
    }
    
    void Transform::rotate(float angle, const vec3f &axis)
    {
        Matrix4 rotate_m;
        rotate_m.setRotation(angle, axis);
        _transform = rotate_m * _transform;
    }
    /*
    void Transform::rotate(const glm::mat3 &)
    {
        //todo
    }
    
    void Transform::setRotation(float angle, const vec3f &axis)
    {
        
    }
    
    void Transform::setRotation(const glm::mat3 &)
    {
        
    }
    */
    void Transform::translate(const vec3f &move)
    {
        Matrix4 translate_m;
        translate_m.setTranslation(move);
        _transform = translate_m * _transform;
    }
    
    void Transform::setTranslate(const vec3f &)
    {
        
    }
    
    void Transform::setScale(const vec3f &)
    {
        
    }
    
    void Transform::scale(const vec3f &scale)
    {
        Matrix4 scale_m;
        scale_m.setScale(scale);
        _transform = scale_m * _transform;
    }
    
    float *Transform::toArray()
    {
        return NULL;
    }
    
    Transform operator *(const Transform &t1, const Transform &t2)
    {
        Matrix4 m = t1._transform * t2._transform;
        return Transform(m);
    }
    
    vec2f Transform::transformVector2(const vec2f &vec)
    {
        vec2f v =  _transform * vec;
        return v;
    }
}