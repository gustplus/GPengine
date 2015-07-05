// GPGame.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GPEngine.h"
#include "GPVector.h"
#include "TestScreen.h"
#include <iostream>
#include <string>
#include "GPRenderer.h"
#include "GPFileStream.h"


using namespace GPEngine3D;

GPEngine::GPEngine(int iXPosition, int iYPosition, int iWidth, int iHeight, const ColorRGBA& rkBackgroundColor):
	Application(iXPosition, iYPosition, iWidth, iHeight, rkBackgroundColor)
{
}




