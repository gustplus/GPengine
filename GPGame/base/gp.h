#ifndef _GP_H_
#define _GP_H_

#include "GPSmartPtr.h"
#include <assert.h>

#define US_NS_GP                             using namespace GPEngine3D

#define GP_NEW                               new
#define GP_NEW_ARRAY                         new
#define GP_DELETE                            delete
#define GP_DELETE_ARRAY                      delete []

#define PTR_NEW(class)                           new SmartPtr<class>

#define DEFINE_SMARTPTR(class, ptrName)      typedef SmartPtr<class> ptrName;

#define LENGTH(A)                            (sizeof(A)/sizeof(A[0]))

#define DISARROW_COPY_AND_ASSIGN(classname) \
classname(const classname &);\
void operator=(const classname &);


#define DEFINE_SINGLETON(T) \
private:\
static T *s_instance;\
T();\
DISARROW_COPY_AND_ASSIGN(T);\
public:\
static T *getInstance();\

#define IMP_SINGLETON(T)\
T* T::s_instance = NULL;\
T *T::getInstance(){\
    if(s_instance == NULL){\
        s_instance = new T();\
    }\
    return s_instance;\
}

#define GP_SAFE_DELETE(T)\
if(T){\
    GP_DELETE T;\
    T = nullptr;\
}


#define GP_ASSERT(ops, msg) \
if (!ops){\
    printf(msg);\
}\
assert(ops)


#define isInstanceOf(class, obj) (dynamic_cast<class>(obj) == nullptr)

typedef unsigned char byte;
typedef unsigned int uint;

#endif