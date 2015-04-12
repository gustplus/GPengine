//
//  GPStack.h
//  GPEngine
//
//  Created by shizhan on 14-10-1.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPStack__
#define __GPEngine__GPStack__

#include "LinkList.h"
#include "GPSmartPtr.h"

namespace GPEngine3D {
    template<class T>
    class Stack{
        Stack()
        {
            _stack = LinkList<SmartPtr<T>>();
        }
        
        ~Stack()
        {
            
        }
        
        void push(T *element)
        {
            _stack.pushback(element);
        }
        
        T pop()
        {
            _stack.popback();
        }
        
    private:
        LinkList<T> _stack;
    };
}

#endif /* defined(__GPEngine__GPStack__) */
