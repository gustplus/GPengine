#pragma once
#include "gpobject.h"

namespace GPEngine3D{

	class KeyHandler:public Object
	{
	private:
		KeyHandler(void);
		static KeyHandler *keyHandler;


	public:
		virtual ~KeyHandler(void);

		static KeyHandler* getInstance(); 

		bool isKeyDown(unsigned char key);

		bool isMouseLeftDown();
		bool isMouseRightDown();

		bool keys[256];                                
		int mouseX;
		int mouseY;

		bool lMouseButton;
		bool rMouseButton;
		bool mMouseButton;

		int rollDir;

		const static int KEY_ESCAPE = 0x1B;
		const static int KEY_LEFT_ARROW = 0x25;
		const static int KEY_RIGHT_ARROW = 0x27;
		const static int KEY_UP_ARROW = 0x26;
		const static int KEY_DOWN_ARROW = 0x28;
		const static int KEY_HOME = 0x24;
		const static int KEY_END = 0x23;
		const static int KEY_PAGE_UP = 0x21;
		const static int KEY_PAGE_DOWN = 0x22;
		const static int KEY_INSERT = 0x2D;
		const static int KEY_DELETE = 0x2E;
		const static int KEY_F1 = 0x70;
		const static int KEY_F2 = 0x71;
		const static int KEY_F3 = 0x72;
		const static int KEY_F4 = 0x73;
		const static int KEY_F5 = 0x74;
		const static int KEY_F6 = 0x75;
		const static int KEY_F7 = 0x76;
		const static int KEY_F8 = 0x77;
		const static int KEY_F9 = 0x78;
		const static int KEY_F10 = 0x79;
		const static int KEY_F11 = 0x7A;
		const static int KEY_F12 = 0x7B;
		const static int KEY_BACKSPACE = 0x08;
		const static int KEY_TAB = 0x09;
		const static int KEY_ENTER = 0x0D;
		const static int KEY_RETURN = 0x0D;

		const static int KEY_SHIFT = 0x0004;
		const static int KEY_CONTROL = 0x0008;
		const static int KEY_ALT = 0;      // not currently handled
		const static int KEY_COMMAND = 0;  // not currently handled

		const static int MOUSE_LEFT_BUTTON = 0;
		const static int MOUSE_MIDDLE_BUTTON = 1;
		const static int MOUSE_RIGHT_BUTTON = 2;
		const static int MOUSE_UP = 0;
		const static int MOUSE_DOWN = 1;

		const static int MODIFIER_CONTROL = 0x0008;
		const static int MODIFIER_LBUTTON = 0x0001;
		const static int MODIFIER_MBUTTON = 0x0010;
		const static int MODIFIER_RBUTTON = 0x0002;
		const static int MODIFIER_SHIFT = 0x0004;
	};
}

