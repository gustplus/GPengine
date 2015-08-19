#ifndef _GPMATRIX4_
#define _GPMATRIX4_

#include "GPVector.h"
#include "gpmath.h"
#include "stdafx.h"

namespace GPEngine3D{
    enum class AXIS{
        X,
        Y,
        Z
    };

	class Matrix4
	{   
	public:
        friend Matrix4 operator*(const Matrix4 &mat0, const Matrix4 &mat1);
        
		Matrix4(void);
        Matrix4(const Matrix4 &);
		~Matrix4(void);

        //Matrix4 operator *(const Matrix4 &mat);

        vec4f operator *(const vec4f &vec) const;

        void indetify();
        
		void setRotation(float angle, const Vector3f &axis);
        
        void setTranslation(const Vector3f &movement);
        
        void setScale(const Vector3f &scale);
        
        void orthf(float left, float right, float bottom, float top, float zNear, float ZFar);
        
        void perspective(float fieldOfViewY, float aspectRatio, float near, float far);
        
        void frustum(float left, float right, float bottom, float top, float zNear, float zFar);
        
        void lookAt(const Vector3f &eye, const Vector3f &to, const Vector3f &up);
        
        void transpose();
        
        void inverse();
        
    //private:
        /* storage
        0  4  8  12
        1  5  9  13
        2  6  10 14
        3  7  11 15
        */
        union{
            float m[16];
            struct
            {
                Point3f xAxis;
                float z0;
                Point3f yAxis;
                float z1;
                Point3f zAxis;
                float z2;

                float tx;
                float ty;
                float tz;
                float z3;
            };    
        };
        
	};

    typedef Matrix4 mat4;
}

#endif

