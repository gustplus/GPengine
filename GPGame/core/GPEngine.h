#ifndef _GPENGINE_
#define _GPENGINE_

#include "resource.h"
#include "GPApplication.h"
#include "GPScreen.h"

class GPEngine : public GPEngine3D::Application
{
public:
	GPEngine(int iXPosition, int iYPosition, int iWidth, int iHeight, const ColorRGBA& rkBackgroundColor);
};
#endif