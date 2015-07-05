#ifndef _SCREEN_H_
#define _SCREEN_H_
#include "GPObject.h"

namespace GPEngine3D{
	class Screen :
		public Object
	{
	public:
		Screen(void);
		virtual ~Screen(void);

		virtual void update(double deltaTime) = 0;
		virtual void present() = 0;
	};
}

#endif