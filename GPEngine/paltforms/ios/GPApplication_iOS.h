//
//  GPApplication_iOS.h
//  GPEngine
//
//  Created by shizhan on 14/11/8.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPApplication_iOS__
#define __GPEngine__GPApplication_iOS__

#include <stdio.h>
#include "GPApplicationProtocol.h"
#include "gp.h"

namespace GPEngine3D {
    class Application : public ApplicationProtocol
    {
    public:
        DEFINE_SINGLETON(Application)
        
        virtual ~Application();
        
        virtual void applicationDidFinishLaunching();
        
        virtual void applicationDidEnterBackGround();
        
        virtual void applicationWillEnterForeGround();
        
        virtual void setTimeInterval(float interval);
        
        virtual void applicationWillTerminate();
        
        virtual int run();
    };
}

#endif /* defined(__GPEngine__GPApplication_iOS__) */
