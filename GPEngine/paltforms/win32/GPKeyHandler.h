#pragma once
#include "gpobject.h"

namespace GPEngine3D{

	class KeyHandler:public Object
	{
	private:
		KeyHandler(void);
		static KeyHandler *keyHandler;
		bool lMouseButton;
		bool rMouseButton;
		bool mMouseButton;


	public:
		friend class WinApplication;
		virtual ~KeyHandler(void);

		static KeyHandler* getInstance(); 

		bool isKeyDown(unsigned char key);

		bool isMouseLeftDown();
		bool isMouseRightDown();

		bool keys[256];                                
		int mouseX;
		int mouseY;

		int rollDir; // > 0ʱ ���ֹ����������Լ�;   < 0ʱ ���ֹ�����������Ļ
	};
}

