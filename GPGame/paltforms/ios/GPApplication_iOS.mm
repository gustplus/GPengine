//
//  GPApplication_iOS.cpp
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPApplication_iOS.h"
#include "GPDirectorCaller_iOS.h"

namespace GPEngine3D {
    
    IMP_SINGLETON(Application)
    
    Application::Application()
    {
        
    }
    
    Application::~Application()
    {
        
    }
    
    void Application::applicationDidFinishLaunching()
    {
        
    }
    
    void Application::applicationDidEnterBackGround()
    {
        
    }
    
    void Application::applicationWillEnterForeGround()
    {
        
    }
    
    void Application::setTimeInterval(float interval)
    {
        [[GPDirectorCaller getInstance] setTimeInterval:interval];
    }
    
    void Application::applicationWillTerminate()
    {
        
    }
    
    int Application::run()
    {
        applicationDidFinishLaunching();
        [[GPDirectorCaller getInstance] startMainLoop];
        return 0;
    }
}