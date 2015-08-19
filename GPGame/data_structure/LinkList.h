#ifndef __LINKLIST__
#define __LINKLIST__

#include "list.h"
#include "gp.h"
#include <iostream>
#include "stdafx.h"

namespace GPEngine3D{
	template<class T>
	class LinkList :
		public List<T>
	{
	public:
		struct ListNode
		{
			ListNode *next;
			T data;
		};

		typedef ListNode *iterator;

		LinkList(void);
		virtual ~LinkList(void);

		uint_32 length() override;

		bool insert(const T& data, int index) override;
		T removeWithIndex(int index) override;
		bool remove(const T&) override;
		int find(const T&) override;
		T &operator[](int index) override;

		void pushback(const T& data) override;
		void pushfront(const T& data) override;
        
        T popback() override;
        T popfront() override;

		iterator begin();
		iterator end();

		void printList();
		
	private:
		ListNode *first;
		ListNode *last;
		//int len;
	};
    
    template<class T>
    LinkList<T>::LinkList(void) : List<T>(0)
    {
        first = GP_NEW ListNode();
        first->next = NULL;
        first->data = NULL;
        last = first;
    }
    
    template<class T>
    LinkList<T>::~LinkList(void)
    {
    }
    
    template<class T>
    uint_32 LinkList<T>::length()
    {
        return List<T>::i_capacity;
    }
    
    template<class T>
    bool LinkList<T>::insert(const T& data, int index)
    {
        if(index > List<T>::i_capacity || index < 0)
        {
            return false;
        }
        ListNode *insert = GP_NEW ListNode();
        insert->data = data;
        ListNode *node = first;
        for(int i = 0; i<index; ++i)
        {
            node = node->next;
        }
        insert->next = node->next;
        node->next = insert;
        
        if(index == List<T>::i_capacity - 1)
        {
            last = insert;
        }
        
        ++List<T>::i_capacity;
        
        return true;
    }
    
    
    
    template<class T>
    T LinkList<T>:: removeWithIndex(int index)
    {
        if(List<T>::i_capacity == 0)
        {
            return NULL;
        }
        if(index  > List<T>::i_capacity || index < 0)
        {
            return NULL;
        }
        ListNode *remove = first;
        ListNode *node = NULL;
        for(int i = 0; i<index - 1; ++i)
        {
            remove = remove->next;
        }
        node = remove;
        remove = remove->next;
        node->next = remove->next;
        
        if(index == List<T>::i_capacity - 1)
        {
            last = node;
        }
        
        --List<T>::i_capacity;
        T temp = remove->data;
        delete remove;
        return temp;
    }
    
    template<class T>
    int LinkList<T>:: find(const T &target)
    {
        if(NULL == target)
            return -1;
        
        ListNode *node = first;
        for(int i = 0; i<List<T>::i_capacity; ++i)
        {
            node = node->next;
            if(node->next->data == target)
            {
                return i;
            }
        }
        return -1;
    }
    
    template<class T>
    bool LinkList<T>:: remove(const T& target)
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
    T& LinkList<T>:: operator[](int index)
    {
        if(0 == List<T>::i_capacity)
        {
            return NULL;
        }
        if(index  > List<T>::i_capacity || index < 0)
        {
            return NULL;
        }
        ListNode *node = first;
        for(int i = 0; i<index + 1; ++i)
        {
            node = node->next;
        }
        
        return node->data;
    }
    
    template<class T>
    void LinkList<T>::printList()
    {
        ListNode *node = first;
        for(int i = 0; i < List<T>::i_capacity; ++i)
        {
            node = node->next;
            std::cout<<node->data<<std::endl;
        }
    }
    
    template<class T>
    void LinkList<T>::pushback(const T& data)
    {
        ListNode *node = GP_NEW ListNode();
        node->data = data;
        node->next = NULL;
        last->next = node;
        ++List<T>::i_capacity;
    }
    
    template<class T>
    void LinkList<T>::pushfront(const T& data)
    {
        ListNode *node = GP_NEW ListNode();
        node->data = data;
        node->next = first->next;
        first->next = node;
        ++List<T>::i_capacity;
    }
    
    template<class T>
    T LinkList<T>::popback()
    {
        return removeWithIndex(List<T>::i_capacity);
    }
    
    template<class T>
    T LinkList<T>::popfront()
    {
        return removeWithIndex(1);
    }
    
    template<class T>
    typename LinkList<T>::iterator LinkList<T>::begin()
    {
        return first->node;
    }
    
    template<class T>
    typename LinkList<T>::iterator LinkList<T>::end()
    {
        return NULL;
    }

}

#endif