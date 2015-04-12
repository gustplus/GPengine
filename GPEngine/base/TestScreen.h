#pragma once
#include "gpscreen.h"
class TestScreen :
	public GPEngine3D::Screen
{
public:
	TestScreen(void);
	virtual ~TestScreen(void);

	virtual void update(double deltaTime);
	virtual void present();
};

