#ifdef GP_PLATFORM_WIN32

#include "stdafx.h"
#include "GPWinApplication.h"
#include "GPKeyHandler.h"
#include "GPVector.h"
#include <cstdio>
#include <math.h>

#include "GPFileStream.h"
#include "GPStringStream.h"
#include "GPGLShaderProgram.h"
#include "GPGLShader.h"
#include "ObjLoader.h"
#include <string>
#include <vector>
#include <iostream>
#include "gp.h"
#include "ArrayList.h"

#pragma warning(disable:4996)//关闭freopen方法不安全的警告 line 303

#define _DEBUG_

using namespace GPEngine3D;


WinApplication* WinApplication::application = NULL; 
WinApplication::EntryPoint WinApplication::Run = NULL;

WinApplication::WinApplication(int iXPosition, int iYPosition, int iWidth, int iHeight, const ColorRGBA& rkBackgroundColor) :
	isFullScreen(true), done(false), active(true), size(iWidth, iHeight), color(rkBackgroundColor)
{
	GetLocalTime(&time);
	startTime = time.wMilliseconds;
}


WinApplication::~WinApplication(void)
{
	delete m_Renderer;
}

bool WinApplication::Init(bool fullScreen){
	return m_Renderer->InitContent(fullScreen);
}

void WinApplication::SetStartScreen(Screen *start)
{
	this->runScreen = start;
}

LRESULT CALLBACK WinApplication::msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	if(application)
	{
		switch (message)                               
		{
		case WM_ACTIVATE:                          
			{
				if (!HIWORD(wParam))               
				{
					//if(!active)
					//application->OnResume();
					application->active = true;			
				}
				else                                   // 最小化状态
				{
					//if(active)
					//application->OnPause();
					application->active = false;
				}
				return 0;
			}
		case WM_SYSCOMMAND:                            // 系统中断
			{
				switch (wParam)                        // 检查系统调用
				{
				case SC_SCREENSAVE:                    // 屏保要运行
				case SC_MONITORPOWER:                // 显示器要进入节电模式
					return 0;                        // 阻止发生
				}
				break;                             
			}
		case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;                            
			}
		case WM_KEYDOWN:                            // 有键按下
			{
				application->keys[wParam] = true;
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
		case WM_KEYUP:                                // 有键放开
			{
				application->keys[wParam] = false;
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
				application->mouseX = ( int )LOWORD( lParam );
				application->mouseY = ( int )HIWORD( lParam );
				KeyHandler::getInstance()->mouseX = ( int )LOWORD( lParam );
				KeyHandler::getInstance()->mouseY = ( int )HIWORD( lParam );
				if(application->lMouseButton)
				{
					//OnMouseDrag(( int )LOWORD( lParam ),( int )HIWORD( lParam ));
				}
				break;
			}
		case WM_LBUTTONDOWN:
			{
				application->lMouseButton = true;
				KeyHandler::getInstance()->lMouseButton = true;
				break;
			}
		case WM_RBUTTONDOWN:
			{
				application->rMouseButton = true;
				KeyHandler::getInstance()->rMouseButton = true;
				break;
			}
		case WM_LBUTTONUP:
			{
				application->lMouseButton = false;
				KeyHandler::getInstance()->lMouseButton = false;
				break;
			}
		case WM_RBUTTONUP:
			{
				application->rMouseButton = false;
				KeyHandler::getInstance()->rMouseButton = false;
				break;
			}
		case WM_MOUSEWHEEL:
			{
				int dir = (int)HIWORD(wParam);
				if(dir > 0)
				{
					application->rollDir = 1;
					KeyHandler::getInstance()->rollDir = 1;
				}
				if(dir < 0)
				{
					application->rollDir = -1;
					KeyHandler::getInstance()->rollDir = -1;
				}
			}
		case WM_SIZE:                                // 调整OpenGL窗口大小
			{
				application->m_Renderer->resizeViewPort(LOWORD(lParam),HIWORD(lParam));
				return 0;
			}
		}
	}

	return DefWindowProc(hWnd,message,wParam,lParam);
}


void WinApplication::setPosition(GPPoint2f & position){
	//SetWindowPos(window, HWND_TOP, (int)position.x,(int)position.y,renderer->GetWidth(), renderer->GetHeight(), SWP_NOSIZE);
}

void WinApplication::Logic(){
	while(!done)                                
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))    
		{
			if (msg.message == WM_QUIT)                
			{
				done = true; 
			}
			else                                
			{
				TranslateMessage(&msg);                
				DispatchMessage(&msg);              
			}
		}
		else                                     
		{
			if (active)                     
			{
				if (KeyHandler::getInstance()->keys[VK_ESCAPE])       
				{
					done=true;   
				}
				else                     
				{
					rollDir = 0;

					GetLocalTime(&time);
					deltaTime = fabs((time.wMilliseconds - startTime)/ 1000.0);
					GetLocalTime(&time);
					startTime = time.wMilliseconds;


					glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
					runScreen->update(deltaTime);
					runScreen->present();
					m_Renderer->SwapBuffer();
				}
			}
			/*if (keys[VK_F1])                   
			{
			keys[VK_F1]=FALSE;                
			KillGLWindow();                    
			fullscreen=!fullscreen;       
			// 重建 OpenGL 窗口
			if (!CreateGLWindow("NeHe's OpenGL 程序框架",640,480,16,fullscreen))
			{
			return 0;                   
			}
			}*/
		}
	}

}


int WinApplication::Main(_In_ HINSTANCE hInstance,
						 _In_opt_ HINSTANCE hPrevInstance,
						 _In_ LPTSTR    lpCmdLine,
						 _In_ int       nCmdShow)
{
	TCHAR szTitle[] = "GPEngine";					// 标题栏文本
	TCHAR szWindowClass[] = "main";

	WNDCLASSEX winInfo;
	winInfo.cbSize = sizeof(WNDCLASSEX);

	winInfo.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winInfo.lpfnWndProc	    = WinApplication::msgProc;
	winInfo.cbClsExtra		= 0;
	winInfo.cbWndExtra		= 0;
	winInfo.hInstance		= hInstance;
	winInfo.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	winInfo.hCursor		    = LoadCursor(NULL, IDC_ARROW);
	winInfo.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winInfo.lpszMenuName	= NULL;
	winInfo.lpszClassName	= szWindowClass;
	winInfo.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (!RegisterClassEx(&winInfo))
	{
		return false;
	}

	HWND window = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 1080, 720, NULL, NULL, hInstance, 0);

	if (!window)
	{
		MessageBox( window, "Could not create a window", "RENDERER ERROR", MB_OK | MB_ICONSTOP );
		return false;
	}

	WinApplication *game = (WinApplication *)application;

	GLRenderer *renderer = new GLRenderer(window, Renderer::FM_RGBA, 16, 8, true, application->size.x, application->size.y);
	renderer->SetClearColor(application->color);
	application->SetRenderer(renderer);
	application->SetWindow(window);

	ShowWindow( window, SW_SHOW );						
	SetForegroundWindow( window );	
	UpdateWindow(window);
	SetFocus( window );

	if(!renderer)
		return -1;

	if(!game->Init(false)){
		return -1;
	}

	const GLubyte *version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("glsl version: %s\n", version);

	/*
	GLShaderProgram *program = new GLShaderProgram();
	GLShader *vShader = new GLShader(GLShader::VERTEX_SHADER);
	vShader->LoadFromFile("./Shaders/GLSL/basic.vert");
	program->AttachShader(vShader);
	GLShader *fShader =  new GLShader(GLShader::FRAG_SHADER);
	fShader->LoadFromFile("./Shaders/GLSL/basic.frag");
	program->AttachShader(fShader);
	program->Link();
	program->Use();
	program->PrintAttributes();
	program->PrintUniforms();
	*/


	/*ArrayList<int> list2;
	list2.Insert(1,0);
	list2.Insert(2,0);
	list2.Insert(0,0);
	list2.Pushfront(1);
	list2.Pushfront(1);

	list2.Pushfront(1);
	list2.Pushfront(1);
	list2.Pushfront(1);
	list2.Pushfront(1);
	list2.Pushfront(1);
	list2.Pushfront(1);
	list2.Pushfront(1);
	list2.Pushfront(1);
	list2.Pushfront(1);
	list2.Pushfront(1);
	list2[0] = 10;
	list2.RemoveWithIndex(2);
	*list2.begin() = 1;
	list2.PrintList();*/
	

	game->Logic();

	return 0;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
#ifdef _DEBUG_
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

	if(WinApplication::Run)
	{
		return WinApplication::Run(hInstance,hPrevInstance,lpCmdLine, nCmdShow);
	}
	return 0;
}

#endif