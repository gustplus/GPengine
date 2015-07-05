#ifndef _GLVIEWIMP_WIN32_H_
#define _GLVIEWIMP_WIN32_H_

//#define _USE_SOFTWARE_RENDERER_

#include "GPGLView.h"
#include "GPVector.h"
#include "GPSmartPtr.h"
#include "GPRect.h"

#ifndef _USE_SOFTWARE_RENDERER_
	#include "GPGLRenderer.h"
#else
	#include "GPSoftwareRenderer.h"
#endif
#include <windows.h>
	
namespace GPEngine3D{

	class GLViewImp : public GLView{
	public:
		static SmartPtr<GLView> createWithRect(/*HINSTANCE hInstance, */Rect &winRect, const char *name = "GPEngine3D");
		static SmartPtr<GLView> createWithFullScreen(/*HINSTANCE hInstance, */const char *name = "GPEngine3D");
		static SmartPtr<GLView> createWithWindow(/*HINSTANCE hInstance, */HWND _hWindow);

		GLViewImp(/*HINSTANCE*/);
		~GLViewImp();

		bool isOpenGLReady() override;
        void swapBuffer() override;
        void resizeView() override;
;
        void pollEvents() override;

        bool initWithRect(Rect &winRect, const char *name) override;
        bool initWithFullScreen(const char *name) override;

        bool initWithWindow(HWND _hWindow);

        bool requestFullScreen();

        void showCursor();
        void hideCursor();

		void setViewSize(int width, int height) override;

        static LRESULT CALLBACK handleEvents(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    protected:
    	HINSTANCE _hInstance;
    	HWND _hWindow;

    	Renderer *_renderer;
    	bool _bReady;
	};
}

#endif