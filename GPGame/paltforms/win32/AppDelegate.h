#include "GPApplication.h"



class AppDelegate : public GPEngine3D::Application
{
	bool applicationDidFinishLaunching() override;
        
    void applicationDidEnterBackGround() override;
    
    void applicationWillEnterForeGround() override;

	void applicationWillTerminate() override;
};