#include "gpscreen.h"
#include "GPSoftwarePipeline.h"
#include "GPRenderStructures.h"
#include "GPMatrix4.h"
#include "GPSoftwareTexture.h"

class TestScreen :
	public GPEngine3D::Screen
{
public:
	TestScreen(void);
	virtual ~TestScreen(void);

	virtual void update(double deltaTime);
	virtual void present();

private:
	GPEngine3D::SoftwarePipeline view;
	
	GPEngine3D::Matrix4 projMat;

	GPEngine3D::PolyObject cube0;
	GPEngine3D::PolyObject cube1;
	GPEngine3D::PolyObject cube2;
	GPEngine3D::PolyObject cube3;

	GPEngine3D::RenderList list0;
	GPEngine3D::RenderList list1;
	GPEngine3D::RenderList list2;
	GPEngine3D::RenderList list3;

	GPEngine3D::Texture2D tex;
};

