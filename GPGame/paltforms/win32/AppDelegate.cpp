#include "AppDelegate.h"
#include "GPDirector.h"
#include "GPGLViewImp_Win32.h"

#include "TestScreen.h"

US_NS_GP;

bool AppDelegate::applicationDidFinishLaunching()
{
	Director *director = Director::getInstance();
	if(!director->getGLView())
	{
		Rect rect = {100, 0, 800, 480};
		auto view = GLViewImp::createWithRect(rect, "GPDemo");
		//auto view = GLViewImp::createWithFullScreen();
		director->setGLView(view);
	}

	director->runWithScene(new TestScreen());
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