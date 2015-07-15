#include "stdafx.h"
#include "GPMatrix4.h"

namespace GPEngine3D{
    Matrix4::Matrix4(void)
    {
        indetify();
    }
    
    Matrix4::Matrix4(const Matrix4 &mat)
    {
        memcpy(m, mat.m, sizeof(m) / sizeof(m[0]));
    }
    
    Matrix4::~Matrix4(void)
    {

    }
    
    void Matrix4::indetify()
    {
        m[0] = 1;
        m[1] = 0;
        m[2] = 0;
        m[3] = 0;

        m[4] = 0;
        m[5] = 1;
        m[6] = 0;
        m[7] = 0;

        m[8] = 0; 
        m[9] = 0; 
        m[10] = 1;
        m[11] = 0;

        m[12] = 0;
        m[13] = 0;
        m[14] = 0;
        m[15] = 1;
    }
    
    void Matrix4::setRotation(float angle, const Vector3f &axis)
    {
        
    }
    
    void Matrix4::setTranslation(const Vector3f &movement)
    {
        m[12] = movement.x;
        m[13] = movement.y;
        m[14] = movement.z;
        m[15] = 1;
    }
    
    void Matrix4::setScale(const Vector3f &scale)
    {
        m[0] = scale.x;
        m[1] = 0;
        m[2] = 0;
        m[3] = 0;

        m[4] = 0;
        m[5] = scale.y;
        m[6] = 0;
        m[7] = 0;

        m[8] = 0; 
        m[9] = 0; 
        m[10] = scale.z;
        m[11] = 0;

        m[12] = 0;
        m[13] = 0;
        m[14] = 0;
        m[15] = 1;
    }
    
    void Matrix4::orthf(float left, float right, float bottom, float top, float zNear, float ZFar)
    {
        
    }
    
    void Matrix4::perspective(float fieldOfViewY, float aspectRatio, float near, float far)
    {
        
    }
    
    void Matrix4::frustum(float left, float right, float bottom, float top, float zNear, float zFar)
    {
       float invWidth = 1.0f / (right - left);
       float invHeight = 1.0f / (top - bottom);
       float invNearFar = 1.0f / (zFar - zNear);
       float twoNear = 2 * zNear;

       m[0] = twoNear * invWidth;
       m[1] = 0;
       m[2] = 0;
       m[3] = 0;

       m[4] = 0;
       m[5] = twoNear * invHeight;
       m[6] = 0;
       m[7] = 0;

       m[8] = (right + left) * invWidth; 
       m[9] = (top + bottom) * invHeight; 
       m[10] = -(zFar + zNear) * invNearFar;
       m[11] = -1;

       m[12] = 0;
       m[13] = 0;
       m[14] = -(2 * zFar * zNear) * invNearFar;
       m[15] = 0;
    }
    
    void Matrix4::transpose()
    {
        swap(m[1], m[4]);
        swap(m[2], m[8]);
        swap(m[3], m[12]);
        swap(m[6], m[9]);
        swap(m[7], m[13]);
        swap(m[11], m[14]);
    }
    
    void Matrix4::inverse()
    {
        
    }
    
    void Matrix4::lookAt(const Vector3f &eye, const Vector3f &to, const Vector3f &up)
    {
        
    }

	vec4f Matrix4::operator *(const vec4f &vec) const
    {
		vec4f ret;
		ret.x = m[0] * vec.x + m[4] * vec.y + m[8] * vec.z + m[12] * vec.w;
		ret.y = m[1] * vec.x + m[5] * vec.y + m[9] * vec.z + m[13] * vec.w;
		ret.z = m[2] * vec.x + m[6] * vec.y + m[10] * vec.z + m[14] * vec.w;
		ret.w = m[3] * vec.x + m[7] * vec.y + m[11] * vec.z + m[15] * vec.w;
        return ret;
    }
    
    
    //friend methods
    Matrix4 &operator*=(Matrix4 &mat0, const Matrix4 &mat1)
    {
        return mat0;
    }
    
    Matrix4 operator*(const Matrix4 &mat0, const Matrix4 &mat1)
    {
        Matrix4 ret;
        return ret;
    }
}










