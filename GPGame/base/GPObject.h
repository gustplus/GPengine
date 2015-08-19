#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "stdafx.h"

namespace GPEngine3D{
	class Object
	{
	public:
		Object(void): use(0){};
		virtual ~Object(void);

		//smart point system
		void release();
		void retain();

	private: 
		int use;
	};
}

#endif