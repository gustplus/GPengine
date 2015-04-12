//
//  GPVector.h
//  GPEngine
//
//  Created by shizhan on 14/11/16.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#ifndef __GPEngine__GPVector__
#define __GPEngine__GPVector__

#include <stdio.h>
#include <vector>
#include "gp.h"
#include <functional>
#include <algorithm> // for std::find


namespace GPEngine3D {
    template <class T>
    class Vector{
    public:
        typedef typename std::vector<T>::iterator iterator;
        typedef typename std::vector<T>::const_iterator const_iterator;
        
        typedef typename std::vector<T>::reverse_iterator reverse_iterator;
        typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;
        
        iterator begin() { return _data.begin(); }
        const_iterator begin() const { return _data.begin(); }
        
        iterator end() { return _data.end(); }
        const_iterator end() const { return _data.end(); }
        
        const_iterator cbegin() const { return _data.cbegin(); }
        const_iterator cend() const { return _data.cend(); }
        
        reverse_iterator rbegin() { return _data.rbegin(); }
        const_reverse_iterator rbegin() const { return _data.rbegin(); }
        
        reverse_iterator rend() { return _data.rend(); }
        const_reverse_iterator rend() const { return _data.rend(); }
        
        const_reverse_iterator crbegin() const { return _data.crbegin(); }
        const_reverse_iterator crend() const { return _data.crend(); }

        Vector<T>()
        : _data()
        {

        }

        explicit Vector<T>(ssize_t capacity)
        : _data()
        {
            reserve(capacity);
        }

        ~Vector<T>()
        {
            clear();
        }

        Vector<T>(const Vector<T>& other)
        {
            _data = other._data;
        }
        
        Vector<T>(Vector<T>&& other)
        {
            _data = std::move(other._data);
        }

        Vector<T>& operator=(const Vector<T>& other)
        {
            if (this != &other) {
                clear();
                _data = other._data;
            }
            return *this;
        }

        Vector<T>& operator=(Vector<T>&& other)
        {
            if (this != &other) {
                clear();
                _data = std::move(other._data);
            }
            return *this;
        }
        
        void reserve(ssize_t n)
        {
            _data.reserve(n);
        }
        
        ssize_t capacity() const
        {
            return _data.capacity();
        }
        
        ssize_t size() const
        {
            return  _data.size();
        }
        
        bool empty() const
        {
            return _data.empty();
        }
        
        ssize_t max_size() const
        {
            return _data.max_size();
        }
        
        ssize_t getIndex(T &object) const
        {
            auto iter = std::find(_data.begin(), _data.end(), object);
            if (iter != _data.end())
                return iter - _data.begin();
            
            return -1;
        }
        
        const_iterator find(T &object) const
        {
            return std::find(_data.begin(), _data.end(), object);
        }
        
        iterator find(T &object)
        {
            return std::find(_data.begin(), _data.end(), object);
        }
        
        T at(ssize_t index) const
        {
            return _data[index];
        }
        
        T front() const
        {
            return _data.front();
        }
        
        T back() const
        {
            return _data.back();
        }
        
        bool contains(T &object) const
        {
            return( std::find(_data.begin(), _data.end(), object) != _data.end() );
        }
        
        bool equals(const Vector<T> &other)
        {
            ssize_t s = this->size();
            if (s != other.size())
                return false;
            
            for (ssize_t i = 0; i < s; i++)
            {
                if (this->at(i) != other.at(i))
                {
                    return false;
                }
            }
            return true;
        }
        
        void pushBack(T object)
        {
            _data.push_back( object );
        }

        void pushBack(const Vector<T>& other)
        {
            for(const auto &obj : other) {
                _data.push_back(obj);
            }
        }
        
        void insert(ssize_t index, T object)
        {
            _data.insert((std::begin(_data) + index), object);
        }
        
        void popBack()
        {
            auto last = _data.back();
            _data.pop_back();
        }
        
        void eraseObject(const T &object, bool removeAll = false)
        {
            if (removeAll)
            {
                for (auto iter = _data.begin(); iter != _data.end();)
                {
                    if ((*iter) == object)
                    {
                        iter = _data.erase(iter);
                    }
                    else
                    {
                        ++iter;
                    }
                }
            }
            else
            {
                auto iter = std::find(_data.begin(), _data.end(), object);
                if (iter != _data.end())
                {
                    _data.erase(iter);
                }
            }
        }
        
        iterator erase(iterator position)
        {
            return _data.erase(position);
        }
        
        iterator erase(iterator first, iterator last)
        {
            return _data.erase(first, last);
        }
        
        iterator erase(ssize_t index)
        {
            auto it = std::next( begin(), index );
            return _data.erase(it);
        }
        
        void clear()
        {
            _data.clear();
        }
        
        void swap(const T &object1, const T &object2)
        {
            ssize_t idx1 = getIndex(object1);
            ssize_t idx2 = getIndex(object2);
            
            GP_ASSERT((idx1>=0 && idx2>=0), "invalid object index");
            
            std::swap( _data[idx1], _data[idx2] );
        }
        
        void swap(ssize_t index1, ssize_t index2)
        {
            std::swap( _data[index1], _data[index2] );
        }
        
        void replace(ssize_t index, T object)
        {
            _data[index] = object;
        }
        
        void reverse()
        {
            std::reverse( std::begin(_data), std::end(_data) );
        }
        
        void shrinkToFit()
        {
            _data.shrink_to_fit();
        }
        
    protected:
        std::vector<T> _data;
    };
}
#endif /* defined(__GPEngine__GPVector__) */
