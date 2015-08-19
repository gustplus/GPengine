#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "stdafx.h"
#include "GPSpatial.h"
namespace GPEngine3D{
    class Geometry :
	public Spatial
    {
    public:
        Geometry(void);
        virtual ~Geometry(void);
    };
}

#endif