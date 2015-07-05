#include "stdafx.h"
#include <cstdio>
#include <math.h>
#include <string>
#include <vector>
#include <iostream>

#include "GPFileStream.h"
#include "GPStringStream.h"
#include "GPGLShaderProgram.h"
#include "GPGLShader.h"
#include "ObjLoader.h"
#include "TestScreen.h"
#include "GPColorRGBA.h"
#include "gp.h"
#include "ArrayList.h"
#include "GPApplication_Win32.h"
#include "GPKeyHandler.h"
#include "GPVector.h"
#include "GPDirector.h"

#ifdef  GP_PLATFORM_WIN32
#pragma warning(disable:4996)//关闭freopen方法不安全的警告
#endif

#define _DEBUG_

US_NS_GP;

Application* Application::application = NULL; 

Application *Application::getInstance()
{
	return Application::application;
}

Application::Application()
{
	Application::application = this;
}

Application::~Application(void)
{
	
}

void Application::setTimeInterval(float interval)
{

}

/*
void Application::setPosition(Point2f & position){
	//SetWindowPos(window, HWND_TOP, (int)position.x,(int)position.y,renderer->GetWidth(), renderer->GetHeight(), SWP_NOSIZE);
}*/

int Application::run(){
	if(!applicationDidFinishLaunching()){
		return 0;
	}

	_startTime = clock();

	Director *director = Director::getInstance();
	SmartPtr<GLView> view = director->getGLView();
	while(!view->willWindowClose())
	{	
		clock_t cur_time = clock();
		_deltaTime = (float)(cur_time - _startTime) / CLOCKS_PER_SEC;
		view->pollEvents();
		director->mainLoop(_deltaTime);
		_startTime = cur_time;
	}
	return (0);
}

#include "GPApplication.h"
#include "GPGLViewImp_Win32.h"
#include "GPDirector.h"
#include "GPSmartPtr.h"
#include "AppDelegate.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
#ifdef _DEBUG_
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif
	AppDelegate app;

	return GPEngine3D::Application::getInstance()->run();
}