#include "stdafx.h"
#include "TestScreen.h"
#include "GPKeyHandler.h"
#include "glfunc.h"

#include "GPVector.h"
#include "types.h"

using namespace GPEngine3D; 

TestScreen::TestScreen(void):
	view(PixelFormat::RGB, 8, 24, true, 800, 480)
{
	view.setClearColor(ColorRGBA(0,0,0,1));

	view.enable(ATTR_FLAG::DEPTH_BUFFER);

	projMat.frustum(-0.01, 0.01, -0.006, 0.006, 0.01, 1000);
	//update(0);

	byte *buf = (byte *)malloc(256 * 256 * 4);
	byte *tmpP = buf;
	for(int row = 0; row < 256; ++row)
	{
		for(int col = 0; col < 256; ++col)
		{
			byte c = ((row & 0x8) ^ (col & 0x8)) * 255;
			*tmpP++ = (GLubyte)c;
			*tmpP++ = (GLubyte)c;
			*tmpP++ = (GLubyte)c;
			*tmpP++ = (GLubyte)255;
		}
	}
	tex.initWithBytes(buf, 256, 256);

	GLubyte colors[] = {255, 0, 0, 255,
						0, 255, 0, 255,
						0, 0, 255, 255,
						255, 255, 255, 255,
						255, 0, 0, 255,
						0, 255, 0, 255,
						0, 0, 255, 255,
						255, 255, 255, 255};

	cube0.vertexAttribColorPointer(false, 0, colors, 8);
	cube1.vertexAttribColorPointer(false, 0, colors, 8);
	cube2.vertexAttribColorPointer(false, 0, colors, 8);
	cube3.vertexAttribColorPointer(false, 0, colors, 8);
}


TestScreen::~TestScreen(void)
{
}

void TestScreen::update(double deltaTime){
	static float offsetY = 0;
	static float offsetX = 0;
	static float offsetZ = -50;

	int x = KeyHandler::getInstance()->mouseX;
	int y = KeyHandler::getInstance()->mouseY;

	offsetZ += KeyHandler::getInstance()->rollDir * deltaTime * 7;

	view.clearBuffer();
	
	Color3b colorR = {255,0,0};
	Color3b colorG = {0,255,0};
	Color3b colorB = {0,0,255};
	Color3b colorW = {255,255,255};
	
	float halfX = 5;
	float halfY = 5;
	float halfZ = 5;

	GLfloat vertices[] = { -halfX + offsetX, halfY - offsetY, halfZ + offsetZ,
						   -halfX + offsetX, -halfY - offsetY, halfZ + offsetZ,
						   halfX + offsetX, halfY - offsetY, halfZ + offsetZ,
						   halfX + offsetX, -halfY - offsetY, halfZ + offsetZ,
							-halfX + offsetX, halfY - offsetY, -halfZ + offsetZ,
							-halfX + offsetX, -halfY - offsetY, -halfZ + offsetZ,
							halfX + offsetX, halfY - offsetY, -halfZ + offsetZ,
							halfX + offsetX, -halfY - offsetY, -halfZ + offsetZ};
	cube0.vertexAttribPosPointer(false, 0, vertices, 8);

	GLfloat vertices1[] = { -halfX - offsetX, halfY + offsetY, halfZ + offsetZ,
						   -halfX - offsetX, -halfY + offsetY, halfZ + offsetZ,
						   halfX - offsetX, halfY + offsetY, halfZ + offsetZ,
						   halfX - offsetX, -halfY + offsetY, halfZ + offsetZ,
							-halfX - offsetX, halfY + offsetY, -halfZ + offsetZ,
							-halfX - offsetX, -halfY + offsetY, -halfZ + offsetZ,
							halfX - offsetX, halfY + offsetY, -halfZ + offsetZ,
							halfX - offsetX, -halfY + offsetY, -halfZ + offsetZ};
	cube1.vertexAttribPosPointer(false, 0, vertices1, 8);

	GLfloat vertices2[] = { -halfX - offsetX, halfY - offsetY, halfZ + offsetZ,
						   -halfX - offsetX, -halfY - offsetY, halfZ + offsetZ,
						   halfX - offsetX, halfY - offsetY, halfZ + offsetZ,
						   halfX - offsetX, -halfY - offsetY, halfZ + offsetZ,
							-halfX - offsetX, halfY - offsetY, -halfZ + offsetZ,
							-halfX - offsetX, -halfY - offsetY, -halfZ + offsetZ,
							halfX - offsetX, halfY - offsetY, -halfZ + offsetZ,
							halfX - offsetX, -halfY - offsetY, -halfZ + offsetZ};
	cube2.vertexAttribPosPointer(false, 0, vertices2, 8);

	GLfloat vertices3[] = { -halfX + offsetX, halfY + offsetY, halfZ + offsetZ,
						   -halfX + offsetX, -halfY + offsetY, halfZ + offsetZ,
						   halfX + offsetX, halfY + offsetY, halfZ + offsetZ,
						   halfX + offsetX, -halfY + offsetY, halfZ + offsetZ,
							-halfX + offsetX, halfY + offsetY, -halfZ + offsetZ,
							-halfX + offsetX, -halfY + offsetY, -halfZ + offsetZ,
							halfX + offsetX, halfY + offsetY, -halfZ + offsetZ,
							halfX + offsetX, -halfY + offsetY, -halfZ + offsetZ};
	cube3.vertexAttribPosPointer(false, 0, vertices3, 8);
	
	transformRenderList(cube0, projMat);
	transformRenderList(cube1, projMat);
	transformRenderList(cube2, projMat);
	transformRenderList(cube3, projMat);

	view.projectionToScreenTransform(cube0);
	view.projectionToScreenTransform(cube1);
	view.projectionToScreenTransform(cube2);
	view.projectionToScreenTransform(cube3);

	GLushort indices[] = { 0, 1, 2, 1, 3, 2,
						   4, 5, 0, 0, 5, 1,
						   2, 3, 6, 3, 6, 7,
						   6, 7, 5, 6, 5, 4,
						   0, 6, 4, 0, 2, 6,
						   1, 5, 3, 3, 5, 7 };

	view.drawElements(cube0, indices , 36);
	view.drawElements(cube1, indices , 36);
	view.drawElements(cube2, indices , 36);
	view.drawElements(cube3, indices , 36);

	view.swapBuffer();
	offsetY = -48 + y * 0.2;
	offsetX = -80 + x * 0.2;
}
void TestScreen::present(){
	
}
