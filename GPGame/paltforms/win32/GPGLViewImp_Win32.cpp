#include "GPGLViewImp_Win32.h"
#include "GPKeyHandler.h"
#include "gp.h"
#include "resource.h"
#include "glfunc.h"

#include "GPDirector.h"

US_NS_GP;

SmartPtr<GLView> GLViewImp::createWithRect(/*HINSTANCE hInstance,*/ Rect &winRect, const char *name)
{
	GLViewImp *view = new GLViewImp();
	view->initWithRect(winRect, name);
	return SmartPtr<GLView>(view);
}

SmartPtr<GLView> GLViewImp::createWithFullScreen(/*HINSTANCE hInstance,*/ const char *name)
{
	GLViewImp *view = new GLViewImp();
	view->initWithFullScreen(name);
	return SmartPtr<GLView>(view);
}

SmartPtr<GLView> GLViewImp::createWithWindow(/*HINSTANCE hInstance,*/ HWND _hWindow)
{
	GLViewImp *view = new GLViewImp();
	view->initWithWindow(_hWindow);
	return SmartPtr<GLView>(view);
}

GLViewImp::GLViewImp(/*HINSTANCE hInstance*/):
	GLView(),
//_hInstance(hInstance),
_hInstance(NULL),
_hWindow(NULL),
_renderer(nullptr),
_bReady(false)
{
	_hInstance = GetModuleHandle(0);
}

GLViewImp::~GLViewImp()
{
	if(_renderer)
	{
		delete _renderer;
	}
}

bool GLViewImp::isOpenGLReady()
{
	return _bReady;
}

void GLViewImp::swapBuffer()
{
	if(_renderer)
	{
		_renderer->swapBuffer();
	}
}

bool GLViewImp::initWithWindow(HWND hWindow)
{
	_hWindow = hWindow;
	RECT kRect;
	GetWindowRect(_hWindow, &kRect);
	int iWidth = (kRect.right-kRect.left);
	int iHeight = (kRect.bottom-kRect.top);
#ifndef _USE_SOFTWARE_RENDERER_
	_renderer = new GLRenderer(PixelFormat::RGBA, 16, 8, true, iWidth, iHeight);
	_bReady = dynamic_cast<GLRenderer *>(_renderer)->initContent(_hWindow);
#else
	_renderer = new SoftwareRenderer(PixelFormat::RGBA, 16, 8, true, iWidth, iHeight);
	_bReady = dynamic_cast<SoftwareRenderer *>(_renderer)->initContent();
#endif
	ShowWindow(_hWindow, SW_SHOW);						
	SetForegroundWindow(_hWindow);	
	UpdateWindow(_hWindow);
	SetFocus(_hWindow);

	 const GLubyte *version = glGetString(GL_SHADING_LANGUAGE_VERSION);
     printf("glsl version: %s\n", version);

	return true;
}

bool GLViewImp::initWithRect(Rect &winRect, const char *name) 
{
	TCHAR szTitle[] ="GPEngine3D";
	TCHAR szWindowClass[] = "main";

	WNDCLASSEX winInfo;
	winInfo.cbSize = sizeof(WNDCLASSEX);

	winInfo.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winInfo.lpfnWndProc	    = GLViewImp::handleEvents;
	winInfo.cbClsExtra		= 0;
	winInfo.cbWndExtra		= 0;
	winInfo.hInstance		= _hInstance;
	winInfo.hIcon			= LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	winInfo.hCursor		    = LoadCursor(NULL, IDC_ARROW);
	winInfo.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winInfo.lpszMenuName	= NULL;
	winInfo.lpszClassName	= szWindowClass;
	winInfo.hIconSm		    = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (!RegisterClassEx(&winInfo))
	{
		return false;
	}

	_hWindow = CreateWindowEx(NULL, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		(int)winRect.x, (int)winRect.y, (int)winRect.width, (int)winRect.height + 38, NULL, NULL, _hInstance, 0);
	if (!_hWindow)
	{
		MessageBox( _hWindow, "Could not create a window", "RENDERER ERROR", MB_OK | MB_ICONSTOP );
		return false;
	}

	return initWithWindow(_hWindow);
}

bool GLViewImp::initWithFullScreen(const char *name) 
{
	RECT kRect;
	GetWindowRect(GetDesktopWindow(),&kRect);
	int iWidth = (kRect.right-kRect.left);
	int iHeight = (kRect.bottom-kRect.top);

	Rect winRect = {0, 0, (float)iWidth, (float)iHeight};
	if(initWithRect(winRect, name))
	{
		UINT uiFlags = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING | SWP_NOZORDER;
		DWORD dwStyle= WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;  
		AdjustWindowRect(&kRect, dwStyle, FALSE);  
		SetWindowPos(_hWindow,HWND_TOP,kRect.left,kRect.top,
			kRect.right-kRect.left,kRect.bottom-kRect.top,uiFlags);
		ShowCursor(FALSE);
		return true;
	}else{
		return false;
	}
}

bool GLViewImp::requestFullScreen()
{
	DWORD        dwExStyle;
	DWORD        dwStyle;
	RECT         kRect;
	UINT uiFlags = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING | SWP_NOZORDER;
	dwExStyle = WS_EX_APPWINDOW;                
	dwStyle= WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;                        
	GetWindowRect(GetDesktopWindow(),&kRect);
	_width = (kRect.right-kRect.left);
	_height = (kRect.bottom-kRect.top);
	AdjustWindowRect(&kRect, dwStyle, FALSE);  
	SetWindowPos(_hWindow, HWND_TOP ,kRect.left,kRect.top,
		kRect.right-kRect.left,kRect.bottom-kRect.top,uiFlags);
	_renderer->resizeViewPort(_width,_height);

	ShowCursor(FALSE);
	return true;
}

void GLViewImp::setViewSize(int width, int height)
{
	GLView::setViewSize(width, height);
	_renderer->resizeViewPort(width, height);
}

void  GLViewImp::resizeView()
{

}

void GLViewImp::showCursor()
{
	ShowCursor(TRUE);
}

void GLViewImp::hideCursor()
{
	ShowCursor(FALSE);
}

void GLViewImp::pollEvents()
{
	static MSG msg;
	if (PeekMessage(&msg,NULL, 0, 0, PM_REMOVE))    
	{
		if (msg.message == WM_QUIT)                
		{
			_bWillWindowClose = true;
		}
		else                                
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
		if (KeyHandler::getInstance()->keys[VK_ESCAPE])       
		{
			_bWillWindowClose = true;
		}
		
		if (KeyHandler::getInstance()->keys[VK_F1])                   
		{
			KeyHandler::getInstance()->keys[VK_F1] = FALSE; 
			requestFullScreen();
		}
}

LRESULT CALLBACK GLViewImp::handleEvents(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	KeyHandler::getInstance()->rollDir = 0;
	switch (message)                               
	{
	case WM_ACTIVATE:
		{
			if (!HIWORD(wParam))
			{
				//if(!active)
				//application->OnResume();
				//application->active = true;
			}
			else
			{
				//if(active)
				//application->OnPause();
				//application->active = false;
			}
			return 0;
		}
	case WM_SYSCOMMAND:                            // ÏµÍ³ÖÐ¶Ï
		{
			switch (wParam)                        // ¼ì²éÏµÍ³µ÷ÓÃ
			{
			case SC_SCREENSAVE:                    // ÆÁ±£ÒªÔËÐÐ
			case SC_MONITORPOWER:                // ÏÔÊ¾Æ÷Òª½øÈë½ÚµçÄ£Ê½
				return 0;                        // ×èÖ¹·¢Éú
			}
			break;                             
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;                            
		}
	case WM_KEYDOWN:                            // ÓÐ¼ü°´ÏÂ
		{
			KeyHandler::getInstance()->keys[wParam] = true;
			if ((VK_F1 <= wParam && wParam <= VK_F12)
				||  (VK_PRIOR <= wParam && wParam <= VK_DOWN)
				||  (wParam == VK_INSERT) || (wParam == VK_DELETE))
			{
				//application->OnSpecialKeyDown(wParam);
			}
			else
			{
				//application->OnKeyDown(wParam);
			}
			return 0;
		}
	case WM_KEYUP:                                // ÓÐ¼ü·Å¿ª
		{
			KeyHandler::getInstance()->keys[wParam] = false;
			if ((VK_F1 <= wParam && wParam <= VK_F12)
				||  (VK_PRIOR <= wParam && wParam <= VK_DOWN)
				||  (wParam == VK_INSERT) || (wParam == VK_DELETE))
			{
				//application->OnSpecialKeyUp(wParam);
			}
			else
			{
				//application->OnKeyUp(wParam);
			}
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			KeyHandler::getInstance()->mouseX = ( int )LOWORD( lParam );
			KeyHandler::getInstance()->mouseY = ( int )HIWORD( lParam );
			if(KeyHandler::getInstance()->lMouseButton)
			{
				//OnMouseDrag(( int )LOWORD( lParam ),( int )HIWORD( lParam ));
			}
			break;
		}
	case WM_LBUTTONDOWN:
		{
			KeyHandler::getInstance()->lMouseButton = true;
			break;
		}
	case WM_RBUTTONDOWN:
		{
			KeyHandler::getInstance()->rMouseButton = true;
			break;
		}
	case WM_LBUTTONUP:
		{
			KeyHandler::getInstance()->lMouseButton = false;
			break;
		}
	case WM_RBUTTONUP:
		{
			KeyHandler::getInstance()->rMouseButton = false;
			break;
		}
	case WM_MOUSEWHEEL:
		{
			int dir = GET_WHEEL_DELTA_WPARAM(wParam);
			if(dir > 0)
			{
				KeyHandler::getInstance()->rollDir = 1;
			}
			if(dir < 0)
			{
				KeyHandler::getInstance()->rollDir = -1;
			}
		}
	case WM_SIZE:
		{
			auto glview = Director::getInstance()->getGLView();
			if(glview.is_valid()){
				glview->setViewSize(LOWORD(lParam),HIWORD(lParam));
			}
			return 0;
		}
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}