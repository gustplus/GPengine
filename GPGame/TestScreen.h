#include "gpscreen.h"
#include "GPSoftwareRenderer.h"
#include "GPRenderStructures.h"
#include "GPMatrix4.h"

class TestScreen :
	public GPEngine3D::Screen
{
public:
	TestScreen(void);
	virtual ~TestScreen(void);

	virtual void update(double deltaTime);
	virtual void present();

private:
	GPEngine3D::SoftwareRenderer view;
	
	GPEngine3D::Matrix4 projMat;

	GPEngine3D::RenderList cube0;
	GPEngine3D::RenderList cube1;
	GPEngine3D::RenderList cube2;
	GPEngine3D::RenderList cube3;
};

