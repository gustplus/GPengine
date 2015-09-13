#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include "GPObject.h"
#include "GPVector.h"
#include "GPMatrix4.h"
#include "types.h"
#include "stdafx.h"

namespace GPEngine3D
{
	enum class VarType
	{
		NIL,
		BYTE,
		UBYTE,
		INT16,
		UINT16,
		INT32,
		UINT32,
		INT64,
		UINT64,

		FLOAT,
		DOUBLE,

		VEC2I,
		VEC2F,

		VEC3I,
		VEC3F,
		VEC4I,
		VEC4F,

		MAT4
	};


	class Variable : 
	 public Object
	 {
	 private:
	 	VarType type;

	 	union{
	 		byte bType;
	 		uchar ubType;
	 		int_16 sType;
	 		uint_16 usType;
	 		int_32 iType;
	 		uint_32 uiType;
	 		int_64 lType;
	 		uint_64 ulType;

	 		float fType;
	 		double dType;

	 		vec2i *v2iType;
	 		vec3i *v3iType;
	 		vec4i *v4iType;

	 		vec2f *v2fType;
	 		vec3f *v3fType;
	 		vec4f *v4fType;

	 		mat4 *mat4Type;
	 	};

	 public:
	 	Variable();

	 	VarType getType() {return type;};


	 	// byte getByte() const;
	 	// ubyte getUByte() const;
	 	// uint_16 getUInt16() const;
	 	// int_16 getInt16() const;
	 	// uint_32 getUint32() const;
	 	// int_32 getInt32() const;
	 	// uint_64 getUInt64() const;
	 	// int_64 getInt64() const;

	 	// float getFloat() const;
	 	// double getDouble() const;

	 	// const vec2i &getVec2i() const;
	 	// const vec2f &getVec2f() const;
	 	// const vec3i &getVec3i() const;
	 	// const vec3f &getVec3f() const;
	 	// const vec4i &getVec4i() const;
	 	// const vec4f &getVec4f() const;

	 	// const mat4 &getMat4() const;

	 	// vec2i &getVec2i();
	 	// vec2f &getVec2f();
	 	// vec3i &getVec3i();
	 	// vec3f &getVec3f();
	 	// vec4i &getVec4i();
	 	// vec4f &getVec4f();

	 	// mat4 &getMat4();
	 };

}

#endif 