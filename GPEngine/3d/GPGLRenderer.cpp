#include "stdafx.h"
#include "GPGLRenderer.h"
#include "glfunc.h"

using namespace GPEngine3D;

GLRenderer::GLRenderer(HWND window, PixelFormat pFormat, char depthBits, char stencilBits, bool doubleBuffer,int width, int height ):
	Renderer(pFormat, cDepthBits, cStencilBits, doubleBuffer, width, height), m_Window(window)
{

}


GLRenderer::~GLRenderer(void)
{
	ReleaseContent();
}

bool GLRenderer::InitContent(bool fullScreen)
{
	if(!(deviceContext = GetDC(m_Window)))
	{
		MessageBox( m_Window, "Could not create a OpenGL device context", "RENDERER ERROR", MB_OK | MB_ICONSTOP );
	}

	PIXELFORMATDESCRIPTOR pixelFormat;
	memset(&pixelFormat, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormat.nVersion = 1;
	pixelFormat.dwFlags =
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_GENERIC_ACCELERATED |
		PFD_DOUBLEBUFFER;
	pixelFormat.iPixelType = PFD_TYPE_RGBA;
	pixelFormat.cColorBits = cStencilBits;   // bit colors for front/back buffers
	pixelFormat.cDepthBits = cDepthBits;     // bits of depth buffer
	pixelFormat.cStencilBits = cStencilBits; // bits of stencil buffer
	pixelFormat.iLayerType = PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(deviceContext,&pixelFormat);
	if ( iPixelFormat == 0 )
	{
		ReleaseDC(m_Window,deviceContext);
		return false;
	}

	bool bSuccess = SetPixelFormat(deviceContext,iPixelFormat,&pixelFormat);
	if ( !bSuccess )
	{
		ReleaseDC(m_Window,deviceContext);
		return false;
	}

	hWindowRC = wglCreateContext(deviceContext);
	if ( !hWindowRC )
	{
		ReleaseDC(m_Window,deviceContext);
		return false;
	}
	bSuccess = wglMakeCurrent(deviceContext,hWindowRC);
	if ( !bSuccess )
	{
		wglDeleteContext(hWindowRC);
		ReleaseDC(m_Window,deviceContext);
		return false;
	}

	GLint err = glewInit();
	if(GLEW_OK != err)
	{
		MessageBox( m_Window, "Error initializing GLEW", "RENDERER ERROR", MB_OK | MB_ICONSTOP );
	}

	if(fullScreen)
		return RequestFullScreen();
	return true;
}

bool GLRenderer::ReleaseContent()
{
	wglDeleteContext(hWindowRC);
	ReleaseDC(m_Window,deviceContext);
	return true;
}

bool GLRenderer::RequestFullScreen()
{
	DWORD        dwExStyle;                     
	DWORD        dwStyle; 
	RECT         kRect;
	UINT uiFlags = SWP_NOACTIVATE | SWP_NOOWNERZORDER |
		SWP_NOSENDCHANGING | SWP_NOZORDER;

	dwExStyle=WS_EX_APPWINDOW;                
	dwStyle= WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;                        
	ShowCursor(FALSE);
	GetWindowRect(GetDesktopWindow(),&kRect);
	iWidth = (kRect.right-kRect.left);
	iHeight = (kRect.bottom-kRect.top);
	AdjustWindowRect(&kRect,dwStyle,FALSE);  
	SetWindowPos(m_Window,HWND_TOP,kRect.left,kRect.top,
		kRect.right-kRect.left,kRect.bottom-kRect.top,uiFlags);
	resizeViewPort(iWidth,iHeight);
	return true;
}

void GLRenderer::ClearBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::SwapBuffer()
{
	SwapBuffers(deviceContext);
}

void GLRenderer::resizeViewPort(int width, int height)
{
	Renderer::resizeViewPort(width, height);
	glViewport(0,0,width, height);
}