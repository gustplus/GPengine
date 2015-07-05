#ifndef _SMARTPTR_
#define _SMARTPTR_

#include "GPObject.h"
#include <assert.h>
#include <cstdio>

namespace GPEngine3D
{
	template<class T>
	class SmartPtr
	{
	public:
		SmartPtr(T* obj = nullptr);
		SmartPtr(const SmartPtr<T> &);
		~SmartPtr(void);

		SmartPtr &operator= (const SmartPtr<T> &obj);
		SmartPtr &operator= (T* obj);
		operator T*();

		bool operator== (const SmartPtr<T> &);
		bool operator!= (const SmartPtr<T> &);

		bool is_valid(){ return ptr != nullptr;};

        T* operator->() const;
        T& operator*() const;

		void debug();

	private:
		void retain();
		void release();

		//int* use;
    protected:
		T *ptr;
	};

    template <class T>
    SmartPtr<T>::SmartPtr(T *obj)
    :ptr(obj)
    {
//        assert(dynamic_cast<Object *>(ptr));
        if(ptr)
        {
            retain();
        }
    }
    
    template <class T>
    SmartPtr<T>::SmartPtr(const SmartPtr<T> &obj)
    :ptr(obj.ptr)
    {
//        assert(dynamic_cast<Object *>(ptr));
        if(ptr)
        {
            retain();
        }
    }
    
    template <class T>
    SmartPtr <T>::~SmartPtr(void)
    {
        release();
    }
    
    template <class T>
    SmartPtr<T> &SmartPtr<T>::operator= (const SmartPtr<T>& obj)
    {
        printf(" = operator \n");
        if(obj.ptr)
        {
            if(ptr != obj.ptr){
                if(obj.ptr)
                {
                    obj.ptr->retain();
                }
                if(ptr)
                {
                    release();
                }
                ptr = obj.ptr;
            }
        }
        else
        {
            if(ptr)
            {
                release();
            }
            ptr = nullptr;
        }
        return *this;
    }
    
    template <class T>
    SmartPtr<T> &SmartPtr<T>::operator= (T* obj)
    {
//        assert(dynamic_cast<Object *>(obj));
        printf(" = ptr : %d\n", obj);
        if(obj)
        {
            if(ptr != obj){
                if(obj)
                {
                    obj->retain();
                }
                if(ptr)
                {
                    release();
                }
                ptr = obj;
            }
        }
        else
        {
            if(ptr)
            {
                release();
            }
            ptr = nullptr;
        }
        return *this;
    }
    
    template <class T>
    bool SmartPtr<T>::operator== (const SmartPtr& obj)
    {
        return obj.ptr == ptr;
    }
    
    template <class T>
    bool SmartPtr<T>::operator!= (const SmartPtr& obj)
    {
        return obj.ptr != ptr;
    }
    
    template <class T>
    T* SmartPtr<T>::operator->() const
    {
        return ptr;
    }
    
    template <class T>
    T &SmartPtr<T>::operator*() const
    {
        return *ptr;
    }
    
    template <class T>
    void SmartPtr<T>::debug()
    {
        //std::cout<<*use<<std::endl;
    }
    
    template <class T>
    void SmartPtr<T>::retain()
    {
        if(ptr)
            ptr->retain();
    }
    
    template <class T>
    void SmartPtr<T>::release()
    {
        if(ptr)
        {
            ptr->release();
        }
    }
    
    template <class T>
    SmartPtr<T>::operator T*()
    {
        return ptr;
    }
}
#endif