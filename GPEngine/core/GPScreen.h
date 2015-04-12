#pragma once
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
