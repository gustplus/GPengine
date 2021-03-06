#ifndef _SPATIAL_
#define _SPATIAL_

#include "stdafx.h"
#include "gpobject.h"
#include "GPTransform.h"
#include "GPSmartPtr.h"

namespace GPEngine3D{
	class Spatial :
		public Object
	{
	public:
		Spatial(void);
		virtual ~Spatial(void);

		void SetParent(Spatial *parent);

	private:
		Transform m_transform;

		Spatial *m_parent;

		
	};

	typedef SmartPtr<Spatial> SpatialPtr;
}
#endif