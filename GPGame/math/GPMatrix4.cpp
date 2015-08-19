#include "stdafx.h"
#include "GPMatrix4.h"
#include "gp.h"
namespace GPEngine3D{
    Matrix4::Matrix4(void)
    {
        indetify();
    }
    
    Matrix4::Matrix4(const Matrix4 &mat)
    {
        m[0] = mat.m[0];
        m[1] = mat.m[1];
        m[2] = mat.m[2];
        m[3] = mat.m[3];

        m[4] = mat.m[4];
        m[5] = mat.m[5];
        m[6] = mat.m[6];
        m[7] = mat.m[7];

        m[8] = mat.m[8]; 
        m[9] = mat.m[9]; 
        m[10] = mat.m[10];
        m[11] = mat.m[11];

        m[12] = mat.m[12];
        m[13] = mat.m[13];
        m[14] = mat.m[14];
        m[15] = mat.m[15];
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
        vec3f axis_sq(axis.x * axis.x, axis.y * axis.y, axis.z * axis.z);

        double cosine = Math::cos(angle);
        double sine = Math::sin(angle);

        m[0] = axis_sq.x + cosine * ( 1.0 - axis_sq.x );
        m[1] = axis.x * axis.y *  ( 1.0 - cosine ) + axis.z * sine;
        m[2] = axis.z * axis.x * ( 1.0 - cosine ) - axis.y * sine;
        m[3] = 0;

        m[4] = axis.x * axis.y * ( 1.0 - cosine ) - axis.z * sine;
        m[5] = axis_sq.y + cosine  * ( 1.0 - axis_sq.y );
        m[6] = axis.y * axis.z * ( 1.0 - cosine ) + axis.x * sine;
        m[7] = 0;

        m[8] = axis.z * axis.x * ( 1.0 - cosine ) + axis.y * sine;
        m[9] = axis.y * axis.z * ( 1.0 - cosine ) - axis.x * sine;
        m[10] = axis_sq.z + cosine  * ( 1.0 - axis_sq.z );
        m[11] = 0;
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
    
    void Matrix4::orthf(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        float invWidth = 1.0f / (right - left);
        float invHeight = 1.0f / (top - bottom);
        float invNearFar = 1.0f / (zFar - zNear);

        m[0] = 2 * invWidth;
        m[1] = 0;
        m[2] = 0;
        m[3] = 0;

        m[4] = 0;
        m[5] = 2 * invHeight;
        m[6] = 0;
        m[7] = 0;

        m[8] = 0;
        m[9] = 0;
        m[10] = 2 * invNearFar;
        m[11] = 0;

        m[12] = -(left + right) * invWidth;
        m[13] = -(top + bottom) * invHeight;
        m[14] = -(zFar + zNear) * invNearFar;
        m[15] = 1;
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
       m[14] = -(twoNear * zFar) * invNearFar;
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
    
 /*  
    Matrix4 Matrix4::operator*(const Matrix4 &mat)
    {
		Matrix4 ret;
        ret.m[0] = m[0] * mat.m[0] + m[4] * mat.m[1] + m[8] * mat.m[2] + m[12] * mat.m[3];
        ret.m[1] = m[1] * mat.m[0] + m[5] * mat.m[1] + m[9] * mat.m[2] + m[13] * mat.m[3];
        ret.m[2] = m[2] * mat.m[0] + m[6] * mat.m[1] + m[10] * mat.m[2] + m[14] * mat.m[3];
        ret.m[3] = m[3] * mat.m[0] + m[7] * mat.m[1] + m[11] * mat.m[2] + m[15] * mat.m[3];

        ret.m[4] = m[0] * mat.m[4] + m[4] * mat.m[5] + m[8] * mat.m[6] + m[12] * mat.m[7];
        ret.m[5] = m[1] * mat.m[4] + m[5] * mat.m[5] + m[9] * mat.m[6] + m[13] * mat.m[7];
        ret.m[6] = m[2] * mat.m[4] + m[6] * mat.m[5] + m[10] * mat.m[6] + m[14] * mat.m[7];
        ret.m[7] = m[3] * mat.m[4] + m[7] * mat.m[5] + m[11] * mat.m[6] + m[15] * mat.m[7];

        ret.m[8] = m[0] * mat.m[8] + m[4] * mat.m[9] + m[8] * mat.m[10] + m[12] * mat.m[11];
        ret.m[9] = m[1] * mat.m[8] + m[5] * mat.m[9] + m[9] * mat.m[10] + m[13] * mat.m[11];
        ret.m[10] = m[2] * mat.m[8] + m[6] * mat.m[9] + m[10] * mat.m[10] + m[14] * mat.m[11];
        ret.m[11] = m[3] * mat.m[8] + m[7] * mat.m[9] + m[11] * mat.m[10] + m[15] * mat.m[11];

        ret.m[12] = m[0] * mat.m[12] + m[4] * mat.m[13] + m[8] * mat.m[14] + m[12] * mat.m[15];
        ret.m[13] = m[1] * mat.m[12] + m[5] * mat.m[13] + m[9] * mat.m[14] + m[13] * mat.m[15];
        ret.m[14] = m[2] * mat.m[12] + m[6] * mat.m[13] + m[10] * mat.m[14] + m[14] * mat.m[15];
        ret.m[15] = m[3] * mat.m[12] + m[7] * mat.m[13] + m[11] * mat.m[14] + m[15] * mat.m[15];
        return ret;
    }
	*/
    //friend methods
    Matrix4 operator*(const Matrix4 &mat0, const Matrix4 &mat1)
    {
        Matrix4 ret;
        ret.m[0] = mat0.m[0] * mat1.m[0] + mat0.m[4] * mat1.m[1] + mat0.m[8] * mat1.m[2] + mat0.m[12] * mat1.m[3];
        ret.m[1] = mat0.m[1] * mat1.m[0] + mat0.m[5] * mat1.m[1] + mat0.m[9] * mat1.m[2] + mat0.m[13] * mat1.m[3];
        ret.m[2] = mat0.m[2] * mat1.m[0] + mat0.m[6] * mat1.m[1] + mat0.m[10] * mat1.m[2] + mat0.m[14] * mat1.m[3];
        ret.m[3] = mat0.m[3] * mat1.m[0] + mat0.m[7] * mat1.m[1] + mat0.m[11] * mat1.m[2] + mat0.m[15] * mat1.m[3];

        ret.m[4] = mat0.m[0] * mat1.m[4] + mat0.m[4] * mat1.m[5] + mat0.m[8] * mat1.m[6] + mat0.m[12] * mat1.m[7];
        ret.m[5] = mat0.m[1] * mat1.m[4] + mat0.m[5] * mat1.m[5] + mat0.m[9] * mat1.m[6] + mat0.m[13] * mat1.m[7];
        ret.m[6] = mat0.m[2] * mat1.m[4] + mat0.m[6] * mat1.m[5] + mat0.m[10] * mat1.m[6] + mat0.m[14] * mat1.m[7];
        ret.m[7] = mat0.m[3] * mat1.m[4] + mat0.m[7] * mat1.m[5] + mat0.m[11] * mat1.m[6] + mat0.m[15] * mat1.m[7];

        ret.m[8] = mat0.m[0] * mat1.m[8] + mat0.m[4] * mat1.m[9] + mat0.m[8] * mat1.m[10] + mat0.m[12] * mat1.m[11];
        ret.m[9] = mat0.m[1] * mat1.m[8] + mat0.m[5] * mat1.m[9] + mat0.m[9] * mat1.m[10] + mat0.m[13] * mat1.m[11];
        ret.m[10] = mat0.m[2] * mat1.m[8] + mat0.m[6] * mat1.m[9] + mat0.m[10] * mat1.m[10] + mat0.m[14] * mat1.m[11];
        ret.m[11] = mat0.m[3] * mat1.m[8] + mat0.m[7] * mat1.m[9] + mat0.m[11] * mat1.m[10] + mat0.m[15] * mat1.m[11];

        ret.m[12] = mat0.m[0] * mat1.m[12] + mat0.m[4] * mat1.m[13] + mat0.m[8] * mat1.m[14] + mat0.m[12] * mat1.m[15];
        ret.m[13] = mat0.m[1] * mat1.m[12] + mat0.m[5] * mat1.m[13] + mat0.m[9] * mat1.m[14] + mat0.m[13] * mat1.m[15];
        ret.m[14] = mat0.m[2] * mat1.m[12] + mat0.m[6] * mat1.m[13] + mat0.m[10] * mat1.m[14] + mat0.m[14] * mat1.m[15];
        ret.m[15] = mat0.m[3] * mat1.m[12] + mat0.m[7] * mat1.m[13] + mat0.m[11] * mat1.m[14] + mat0.m[15] * mat1.m[15];
        return ret;
    }
}










