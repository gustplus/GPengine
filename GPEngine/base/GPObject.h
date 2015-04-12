#ifndef _OBJECT_
#define _OBJECT_

namespace GPEngine3D{
	class Object
	{
	public:
		Object(void): use(0){};
		virtual ~Object(void);

		//smart point system
		void release();
		void retain();

	private: 
		int use;
	};
}

#endif