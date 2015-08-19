#ifndef _TRANSFORM_
#define _TRANSFORM_

#include "gpobject.h"
#include "GPVector.h"
#include "stdafx.h"
#include "GPmatrix4.h"

namespace GPEngine3D{
	class Transform :
		public Object
	{
	public:
		friend Transform operator *(const Transform &t1, const Transform &t2);

		Transform(void);
        Transform(Matrix4 &);
		virtual ~Transform(void);
		
		void rotate(float angle, const vec3f &axis);
/*
		void rotate(const glm::mat3 &);

		void setRotation(float angle, const vec3f &axis);

		void setRotation(const glm::mat3 &);
		*/
		void translate(const vec3f &);

		void setTranslate(const vec3f &);

		void setScale(const vec3f &);

		void scale(const vec3f &);
        
        float *toArray();
        
        vec4f transformVector3(vec3f &vec);

	private:
//		glm::mat3 rotate;
//		vec3f translate;
//		vec3f scale;

        Matrix4 _transform;
    };
}
#endif