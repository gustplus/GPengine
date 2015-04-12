#include "stdafx.h"
#include "TestScreen.h"
#include "GPKeyHandler.h"
#include "GPWinApplication.h"
#include "glfunc.h"
#include "GPSmartPtr.h"
#include "GPObject.h"

using namespace GPEngine3D; 

TestScreen::TestScreen(void)
{
}


TestScreen::~TestScreen(void)
{
}

void TestScreen::update(double deltaTime){
	int x = KeyHandler::getInstance()->mouseX;
	int y = KeyHandler::getInstance()->mouseY;
	float b = (float)x/1080;
	float r= (float)y/720;
	glClearColor(r,0,b,1);
	if(KeyHandler::getInstance()->isMouseLeftDown())
		glClearColor(0,0,KeyHandler::getInstance()->rollDir,1);
}
void TestScreen::present(){
	
}
