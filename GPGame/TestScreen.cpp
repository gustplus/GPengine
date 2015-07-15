#include "stdafx.h"
#include "TestScreen.h"
#include "GPKeyHandler.h"
#include "glfunc.h"

#include "GPVector.h"

using namespace GPEngine3D; 

TestScreen::TestScreen(void):
	view(PixelFormat::RGB, 8, 24, true, 800, 480)
{
	view.setClearColor(ColorRGBA(0,0,0,1));
}


TestScreen::~TestScreen(void)
{
}

void TestScreen::update(double deltaTime){
	static float offsetY = 0;
	static float offsetX = 0;
	static float offsetZ = -20;

	int x = KeyHandler::getInstance()->mouseX;
	int y = KeyHandler::getInstance()->mouseY;
	//float b = (float)x/1080;
	//float r= (float)y/720;
	//glClearColor(r,0,b,1);

	//if(KeyHandler::getInstance()->isMouseLeftDown())
		offsetZ += KeyHandler::getInstance()->rollDir * deltaTime;
	
	
	view.clearBuffer();
	
	Color3b colorR = {255,0,0};
	Color3b colorG = {0,255,0};
	Color3b colorB = {0,0,255};
	Color3b colorW = {255,255,255};
	/*
	int offsetX = 0;
	int offsetY = 0; 
	for(int i = 0; i < 100; ++i){
		for(int j = 0; j < 100; ++j){
			view.drawTriangleSolid(0 + offsetX, 0 + offsetY, 10 + offsetX, 0 + offsetY, 0 + offsetX, 10 + offsetY, colorW);
			view.drawLine(0 + offsetX, 0 + offsetY, 10 + offsetX, 0 + offsetY, colorR);
			view.drawLine(10 + offsetX, 0 + offsetY, 0 + offsetX, 10 + offsetY, colorG);
			view.drawLine(0 + offsetX, 0 + offsetY, 0 + offsetX, 10 + offsetY, colorB);
			offsetY = 10 * j;
		}
		offsetX = 10 * i;
	}
	
	//view.drawTriangleSolid(0, 0, 0, 480, 800, 480, colorW);
	//view.drawTriangleSolid(0, 0, 800, 0, 800, 480, colorB);
		
	view.drawTriangleSolid(200, 100, 100, 100, 100, 200, colorR);
	view.drawTriangleSolid(200, 0, 300, 0, 400, 100, colorG);
	view.drawTriangleSolid(200, 200, 300, 200, 300, 300, colorB);
	*/
	
	Matrix4 mat;
	mat.frustum(-1, 1, -0.6, 0.6, 0.1, 100);

	//GLfloat vertices1[] = {-20, -20, -10 - offset, 20, -20, -10 - offset, -20, 20, -10 - offset}; 
	//view.drawArray(vertices1, mat, 3, colorB);

	GLfloat vertices2[] = {100, -100, -20, 
						   100, -100, -40, 
						   100, -40, -20}; 
	//view.drawArray(vertices2, mat, 3, colorG);

	//view.drawTriangleSolid(600, 39, 500, 139, 600, 159, colorW);
	
	float halfX = 10;
	float halfY = 10;
	float halfZ = 10;

	GLfloat vertices[] = { -halfX + offsetX, halfY - offsetY, halfZ + offsetZ,
						   -halfX + offsetX, -halfY - offsetY, halfZ + offsetZ,
						   halfX + offsetX, halfY - offsetY, halfZ + offsetZ,
						   halfX + offsetX, -halfY - offsetY, halfZ + offsetZ,
							-halfX + offsetX, halfY - offsetY, -halfZ + offsetZ,
							-halfX + offsetX, -halfY - offsetY, -halfZ + offsetZ,
							halfX + offsetX, halfY - offsetY, -halfZ + offsetZ,
							halfX + offsetX, -halfY - offsetY, -halfZ + offsetZ};
	GLushort indices[] = { 0, 1, 2, 1, 2, 3, 4, 5, 0, 0, 5, 1, 2, 3, 6, 3, 6, 7, 6, 7, 5, 6, 5, 4, 0, 6, 4, 0, 2, 6, 1, 5, 3, 3, 5, 7 };
	
	//view.drawElements(vertices, indices, mat, 36, colorG);
	
	view.swapBuffer();
	offsetY = -24 + y * 0.1;
	offsetX = -40 + x * 0.1;
}
void TestScreen::present(){
	
}
