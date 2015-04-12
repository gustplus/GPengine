#include "stdafx.h"
#include "GPObject.h"
#include <cstdio>

using namespace GPEngine3D;

Object::~Object(void)
{
//	release();
}

void Object::release()
{
	--use;
    printf("count %d\n", use);
    if(use<1)
    {
        printf("delete\n");
        delete this;
    }
}


void Object::retain()
{
	++use;
    printf("count %d\n", use);
}
