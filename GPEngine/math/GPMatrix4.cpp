#include "stdafx.h"
#include "GPMatrix4.h"

namespace GPEngine3D{
    Matrix4::Matrix4(void)
    {
        _matrix = glm::mat4(1.0);
    }
    
    Matrix4::Matrix4(const Matrix4 &m)
    {
//        _matrix = m._matrix;
    }
    
    Matrix4::~Matrix4(void)
    {
    }
    
    void Matrix4::indetify()
    {
        _matrix = glm::mat4(1.0);
    }
    
    void Matrix4::setRotation(float angle, const Vector3f &axis)
    {
        _matrix = glm::rotate(_matrix, angle, glm::vec3(axis.x, axis.y, axis.z));
    }
    
    void Matrix4::setTranslation(const Vector3f &movement)
    {
        _matrix = glm::translate(_matrix, glm::vec3(movement.x, movement.y, movement.z));
    }
    
    void Matrix4::setScale(const Vector3f &scale)
    {
        _matrix = glm::scale(_matrix, glm::vec3(scale.x , scale.y, scale.z));
    }
    
    void Matrix4::orthf(float left, float right, float bottom, float top, float zNear, float ZFar)
    {
        _matrix = glm::ortho(left, right, bottom, top, zNear, ZFar);
    }
    
    void Matrix4::perspective(float fieldOfViewY, float aspectRatio, float near, float far)
    {
        _matrix = glm::perspective(fieldOfViewY, aspectRatio, near, far);
    }
    
    void Matrix4::frustum(float left, float right, float bottom, float top, float zNear, float ZFar)
    {
        _matrix = glm::frustum(left, right, bottom, top, zNear, ZFar);
    }
    
    void Matrix4::transpose()
    {
        _matrix = glm::transpose(_matrix);
    }
    
    void Matrix4::inverse()
    {
        _matrix = glm::inverse(_matrix);
    }
    
    Matrix4::operator float *()
    {
        return glm::value_ptr(_matrix);
    }
    
    void Matrix4::lookAt(const Vector3f &eye, const Vector3f &to, const Vector3f &up)
    {
        _matrix = glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(to.x, to.y, to.z), glm::vec3(up.x, up.y, up.z));
    }
    
    
    
    //friend methods
    Matrix4 &operator*=(Matrix4 &mat0, const Matrix4 &mat1)
    {
        mat0._matrix = mat0._matrix * mat1._matrix;
        return mat0;
    }
    
    Matrix4 operator*(const Matrix4 &mat0, const Matrix4 &mat1)
    {
        Matrix4 ret;
        ret._matrix = mat0._matrix * mat1._matrix;
        return ret;
    }
    
    vec3f operator *(const Matrix4 &mat, const vec3f &vec)
    {
        glm::vec4 mv_ret = mat._matrix * glm::vec4(vec.x, vec.y, vec.z, 1);
        vec3f ret(mv_ret.x, mv_ret.y, mv_ret.z);
        return ret;
    }
    
    vec2f operator *(const Matrix4 &mat, const vec2f &vec)
    {
        glm::vec4 mv_ret = mat._matrix * glm::vec4(vec.x, vec.y, 0, 1);
        vec2f ret(mv_ret.x, mv_ret.y);
        return ret;
    }
}










