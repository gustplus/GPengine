//
//  GPApplicateionProtocol.h
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPApplicateionProtocol__
#define __GPEngine__GPApplicateionProtocol__

#include <stdio.h>

namespace GPEngine3D
{
    class ApplicationProtocol
    {
    public:
        virtual ~ApplicationProtocol()
        {
            
        }
        
        virtual void applicationDidFinishLaunching() = 0;
        
        virtual void applicationDidEnterBackGround() = 0;
        
        virtual void applicationWillEnterForeGround() = 0;
        
        virtual void setTimeInterval(float interval) = 0;
        
        virtual int run() = 0;
        
        virtual void applicationWillTerminate() = 0;
    };
}
#endif /* defined(__GPEngine__GPApplicateionProtocol__) */
