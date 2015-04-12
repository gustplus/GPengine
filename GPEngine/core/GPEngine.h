#ifndef _GPENGINE_
#define _GPENGINE_

#include "resource.h"
#include "GPWinApplication.h"
#include "GPScreen.h"

class GPEngine : public GPEngine3D::WinApplication
{
public:
	GPEngine(int iXPosition, int iYPosition, int iWidth, int iHeight, const ColorRGBA& rkBackgroundColor);

	virtual void SetStartScreen(GPEngine3D::Screen *);

	static bool Init();
	
	static int Run(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow);

	static bool initlize; 
};
#endif