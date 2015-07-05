#include "stdafx.h"
#include "TestScreen.h"
#include "GPKeyHandler.h"
#include "glfunc.h"

#include "GPVector.h"

using namespace GPEngine3D; 

TestScreen::TestScreen(void):
	view(PixelFormat::RGB, 8, 24, true, 800, 480)
{
	view.setClearColor(ColorRGBA(0,0,1,1));
}


TestScreen::~TestScreen(void)
{
}

void TestScreen::update(double deltaTime){
	/*int x = KeyHandler::getInstance()->mouseX;
	int y = KeyHandler::getInstance()->mouseY;
	float b = (float)x/1080;
	float r= (float)y/720;
	glClearColor(r,0,b,1);
	if(KeyHandler::getInstance()->isMouseLeftDown())
		glClearColor(0,0,KeyHandler::getInstance()->rollDir,1);
	*/
	
	view.clearBuffer();
	
	Color3b color = {255,0,0};
	Color3b color_w = {255,255,255};

	static float offset = 0;
	for(int i = 0; i < 50; ++i){
		view.drawTriangleSolid(200 + offset, 100, 100 + offset, 101, 100 + offset, 200, color);
		//view.drawLine(200 + offset, 100, 100 + offset, 400, color_w);
		//view.drawLine(300 + offset, 300, 100 + offset, 400, color_w);
		//view.drawLine(200 + offset, 100, 300 + offset, 300, color_w);

		offset = 100 * i;
	}
	view.swapBuffer();
	
	//offset += 100 * deltaTime;
}
void TestScreen::present(){
	
}
