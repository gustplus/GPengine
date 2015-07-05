//
//  GPTimer.cpp
//  GPEngine
//
//  Created by shizhan on 14-10-5.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPTimer.h"
#import <Foundation/Foundation.h>

namespace GPEngine3D {
    
    double Timer::getNanoTime()
    {
        double time = [[NSDate date] timeIntervalSince1970];
        return time;
    }
}