#include "AppDelegate.h"
#include "GPDirector.h"
#include "GPGLViewImp_Win32.h"

#include "SoftwareRendererScreen.h"
#include "GLWaveScreen.h"
#include "GLGemotryShaderScene.h"
#include "GLFilterScreen.h"
#include "GLSkyBoxScreen.h"

US_NS_GP;

bool AppDelegate::applicationDidFinishLaunching()
{
	Director *director = Director::getInstance();
	if(!director->getGLView())
	{
		Rect rect = {100, 0, 800, 480};
		auto view = GLViewImp::createWithRect(rect, "GPDemo");
		// auto view = GLViewImp::createWithFullScreen();
		director->setGLView(view);
	}

	//director->runWithScene(new SoftwareRendererScreen());
	//director->runWithScene(new GLGemotryShaderScene());
	//director->runWithScene(new GLTestScreen());
	//director->runWithScene(new GLFilterScreen());
	director->runWithScene(new GLSkyBoxScreen());
	return true;
}

void AppDelegate::applicationDidEnterBackGround()
{

}

void AppDelegate::applicationWillEnterForeGround()
{

}

void AppDelegate::applicationWillTerminate()
{

}