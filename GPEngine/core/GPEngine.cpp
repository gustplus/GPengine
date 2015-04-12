// GPGame.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GPEngine.h"
#include "GPWinApplication.h"
#include "GPVector.h"
#include "GPVector.cpp"
#include "TestScreen.h"
#include <iostream>
#include <string>
#include "GPRenderer.h"
#include "GPFileStream.h"


using namespace GPEngine3D;

GPEngine::GPEngine(int iXPosition, int iYPosition, int iWidth, int iHeight, const ColorRGBA& rkBackgroundColor):
	WinApplication(iXPosition, iYPosition, iWidth, iHeight, rkBackgroundColor)
{
}

void GPEngine::SetStartScreen(GPEngine3D::Screen *start){
	runScreen = start;
}

bool GPEngine::Init(){
	WinApplication::Run = &GPEngine::Run;
	application = new GPEngine(0, 0, 1080, 720, ColorRGBA(1,1,1,1));
	application->SetStartScreen(new TestScreen());

	return true;
}

bool initlize = GPEngine::Init();

int GPEngine::Run(_In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow)
{
	WinApplication *app = (WinApplication *)application;
	return app->Main( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
}

