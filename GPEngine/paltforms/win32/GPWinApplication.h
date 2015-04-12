#pragma once
#ifdef PLATFORM_WIN32
#include <windows.h> 
#include "GPVector.h"
#include "resource.h"
#include "glfunc.h"
#include "GPScreen.h"
#include "GPGLRenderer.h"

namespace GPEngine3D{
	class WinApplication
	{
	public:
		WinApplication(int iXPosition, int iYPosition, int iWidth, int iHeight, const ColorRGBA& rkBackgroundColor);
		~WinApplication(void);

		static LRESULT CALLBACK msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		bool Init(bool fullScreen);

		void setPosition(GPPoint2f & position);

		void Logic();

		void SetRenderer(Renderer *renderer){m_Renderer = renderer;}

		void SetWindow(HWND window){this->window = window;}

		virtual void SetStartScreen(Screen *) = 0;

		static WinApplication *application;

		typedef int (*EntryPoint)(_In_ HINSTANCE hInstance,
						 _In_opt_ HINSTANCE hPrevInstance,
						 _In_ LPTSTR    lpCmdLine,
						 _In_ int       nCmdShow);

		static EntryPoint Run;

		virtual int Main(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow);

		

		const static int KEY_ESCAPE = VK_ESCAPE;
		const static int KEY_LEFT_ARROW = VK_LEFT;
		const static int KEY_RIGHT_ARROW = VK_RIGHT;
		const static int KEY_UP_ARROW = VK_UP;
		const static int KEY_DOWN_ARROW = VK_DOWN;
		const static int KEY_HOME = VK_HOME;
		const static int KEY_END = VK_END;
		const static int KEY_PAGE_UP = VK_PRIOR;
		const static int KEY_PAGE_DOWN = VK_NEXT;
		const static int KEY_INSERT = VK_INSERT;
		const static int KEY_DELETE = VK_DELETE;
		const static int KEY_F1 = VK_F1;
		const static int KEY_F2 = VK_F2;
		const static int KEY_F3 = VK_F3;
		const static int KEY_F4 = VK_F4;
		const static int KEY_F5 = VK_F5;
		const static int KEY_F6 = VK_F6;
		const static int KEY_F7 = VK_F7;
		const static int KEY_F8 = VK_F8;
		const static int KEY_F9 = VK_F9;
		const static int KEY_F10 = VK_F10;
		const static int KEY_F11 = VK_F11;
		const static int KEY_F12 = VK_F12;
		const static int KEY_BACKSPACE = VK_BACK;
		const static int KEY_TAB = VK_TAB;
		const static int KEY_ENTER = VK_RETURN;
		const static int KEY_RETURN = VK_RETURN;

		const static int KEY_SHIFT = MK_SHIFT;
		const static int KEY_CONTROL = MK_CONTROL;
		const static int KEY_ALT = 0;      // not currently handled
		const static int KEY_COMMAND = 0;  // not currently handled

		const static int MOUSE_LEFT_BUTTON = 0;
		const static int MOUSE_MIDDLE_BUTTON = 1;
		const static int MOUSE_RIGHT_BUTTON = 2;
		const static int MOUSE_UP = 0;
		const static int MOUSE_DOWN = 1;

		const static int MODIFIER_CONTROL = MK_CONTROL;
		const static int MODIFIER_LBUTTON = MK_LBUTTON;
		const static int MODIFIER_MBUTTON = MK_MBUTTON;
		const static int MODIFIER_RBUTTON = MK_RBUTTON;
		const static int MODIFIER_SHIFT = MK_SHIFT;


	protected:
		Renderer* m_Renderer;

		ColorRGBA color;

		GPPoint2f position;

		bool isFullScreen;
		bool done;
		bool active;

		Screen *runScreen;
		Size size;

		HWND window;

		__int64 startTime;
		double deltaTime;

		SYSTEMTIME time;
		MSG msg;

		//输入记录
		bool keys[256];                                
		int mouseX;
		int mouseY;

		bool lMouseButton;
		bool rMouseButton;
		bool mMouseButton;

		int rollDir; // > 0时 滚轮滚动方向向自己;   < 0时 滚轮滚动方向向屏幕
	};


}
#endif
