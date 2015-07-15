#ifndef _ARRAYLIST_
#define _ARRAYLIST_

#include "list.h"
#include "gp.h"
#include <iostream>

namespace GPEngine3D{
	template<class T>
	class ArrayList :
		public List<T>
	{
	public:
		typedef T *iterator;

		ArrayList(int capacity = 10, int increaceSize = 10);
		virtual ~ArrayList(void);

		bool insert(const T* data, int length, int index);
		bool replace(const T* data, int length, int index);

		T* toArray();
		virtual int length();

		virtual bool insert(const T& data, int index);
		virtual T removeWithIndex(int index);
		virtual bool remove(const T&);
		virtual int find(const T&);
		virtual T& operator[](int index);
		virtual const T &operator[](int index) const;

		virtual void pushback(const T& data);
		virtual void pushfront(const T& data);
		iterator begin();
		iterator end();

		void printList();
		

	private:
		void resize(int newSize);

		int i_size;

		int i_increaseSize;

		T *array;
	};
    
    template<class T>
    ArrayList<T>::ArrayList(int capacity, int increaceSize) : List<T>(capacity), i_size(0), i_increaseSize(increaceSize)
    {
        array = GP_NEW T[List<T>::i_capacity];
    }
    
    template<class T>
    ArrayList<T>::~ArrayList(void)
    {
        GP_DELETE array;
    }
    
    template<class T>
    void ArrayList<T>::resize(int newSize)
    {
        T *newArray = GP_NEW T[newSize];
        memcpy(newArray, array, i_size * sizeof(T));
        List<T>::i_capacity = newSize;
        GP_DELETE array;
        array = newArray;
    }
    
    template<class T>
    T* ArrayList<T>::toArray()
    {
        return array;
    }
    
    template<class T>
    int ArrayList<T>::length()
    {
        return i_size;
    }
    
    template<class T>
    bool ArrayList<T>::insert(const T* data, int length, int index)
    {
        std::cout<<i_size<<std::endl;
        std::cout<<List<T>::i_capacity<<std::endl;
        
        if(index >i_size || index < 0)
        {
            return false;
        }
        while(i_size + length > List<T>::i_capacity)
        {
            resize(List<T>::i_capacity+i_increaseSize);
        }
        if(index == i_size)
        {
            std::cout<<"no move"<<std::endl;
        }
        else
        {
            std::cout<<"move"<<std::endl;
            int moveSize = i_size - index;
            moveSize *= sizeof(T);
            memmove(array + index + length, array + index, moveSize);
        }
        memcpy(array + index, data, length);
        i_size += length;
        
        return true;
    }
    
    template<class T>
    bool ArrayList<T>::replace(const T* data, int length, int index)
    {
        if(index >i_size || index < 0)
        {
            return false;
        }
        while(i_size + length > List<T>::i_capacity)
        {
            resize(List<T>::i_capacity+i_increaseSize);
        }
        
        memcpy(array + index, data, length);
        
        return true;
    }
    
    
    template<class T>
    bool ArrayList<T>::insert(const T& data, int index)
    {
        std::cout<<i_size<<std::endl;
        std::cout<<List<T>::i_capacity<<std::endl;
        if(index >i_size || index < 0)
        {
            return false;
        }
        if(i_size == List<T>::i_capacity)
        {
            std::cout<<"resize"<<std::endl;
            resize(List<T>::i_capacity+i_increaseSize);
        }
        if(index == i_size)
        {
            std::cout<<"no move"<<std::endl;
        }
        else
        {
            std::cout<<"move"<<std::endl;
            int moveSize = i_size - index;
            moveSize *= sizeof(T);
            memmove(array + index + 1, array + index, moveSize);
        }
        array[index] = data;
        ++i_size;
        
        return true;
    }
    
    
    
    template<class T>
    T ArrayList<T>:: removeWithIndex(int index)
    {
        if(index  > i_size-1 || index < 0)
        {
            return NULL;
        }
        if(index == i_size-1)
        {
            array[index] = NULL;
        }
        
        T remove = array[index];
        int moveSize = i_size - index - 1;
        moveSize *= sizeof(T);
        memmove_s(array + index, moveSize, array + index + 1, moveSize);
        --i_size;
        return remove;
    }
    
    template<class T>
    int ArrayList<T>:: find(const T &target)
    {
        if(NULL == target)
            return -1;
        
        for(int i = 0; i_size; ++i)
        {
            if(array[i] == target)
            {
                return i;
            }
        }
        return -1;
    }
    
    template<class T>
    bool ArrayList<T>:: remove(const T& target)
    {
        int index = find(target);
        if(-1 != index)
        {
            removeWithIndex(index);
            return true;
        }
        return false;
    }
    
    template<class T>
    T& ArrayList<T>:: operator[](int index)
    {
		return const_cast<T>(static_cast<const ArrayList>(*this)[index]);
    }

	template<class T>
    const T& ArrayList<T>:: operator[](int index) const
    {
        if(index > i_size){
			size_t newSize = i_size;
			while ((newSize += i_increaseSize) < index ){}
			resize(newSize);
		}
		return array[index];
    }
    
    template<class T>
    void ArrayList<T>::printList()
    {
        for(int i = 0; i < i_size; ++i)
        {
            std::cout<<array[i]<<std::endl;
        }
    }
    
    template<class T>
    void ArrayList<T>::pushback(const T& data)
    {
        insert(data, i_size);
    }
    
    template<class T>
    void ArrayList<T>::pushfront(const T& data)
    {
        insert(data, 0);
    }
    
    template<class T>
    typename ArrayList<T>::iterator ArrayList<T>::begin()
    {
        return array;
    }
    
    template<class T>
    typename ArrayList<T>::iterator ArrayList<T>::end()
    {
        return array + i_size;
    }
}

#endif