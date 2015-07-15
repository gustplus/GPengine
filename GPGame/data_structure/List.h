#ifndef __LIST__
#define __LIST__

namespace GPEngine3D{
	template<class T>
	class List
	{
	public:
		List(int capacity = 10):i_capacity(capacity){}
		~List(void){}

		virtual bool insert(const T& data, int index) = 0;
		virtual bool remove(const T&) = 0;
		virtual T removeWithIndex(int) = 0;
		virtual int find(const T&) = 0;
		virtual T &operator[](int index) = 0;
		virtual const T &operator[](int index) const = 0;
		virtual int size(){return i_capacity;}

	    virtual void pushback(const T& data) = 0;
		virtual void pushfront(const T& data) = 0;
        
        virtual T popback() = 0;
        virtual T popfront() = 0;

		virtual int length() = 0;

	protected:
		int i_capacity;
	};

}
#endif