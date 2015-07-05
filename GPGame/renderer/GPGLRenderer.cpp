#include "stdafx.h"
#include "renderer/GPGLRenderer.h"
#include "glfunc.h"
#include "gp.h"

US_NS_GP;

GLRenderer::GLRenderer(PixelFormat pFormat, char depthBits, char stencilBits, bool doubleBuffer,int width, int height ):
	Renderer(pFormat, depthBits, stencilBits, doubleBuffer, width, height)
{
}


GLRenderer::~GLRenderer(void)
{
	releaseContent();
}

bool GLRenderer::initContent(HWND hWindow)
{
	_hWindow = hWindow;
	if(!(_hDeviceContext = GetDC(hWindow)))
	{
		MessageBox( hWindow, "Could not create a OpenGL device context", "RENDERER ERROR", MB_OK | MB_ICONSTOP );
		return false;
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

	int iPixelFormat = ChoosePixelFormat(_hDeviceContext, &pixelFormat);
	if ( iPixelFormat == 0 )
	{
		ReleaseDC(hWindow, _hDeviceContext);
		return false;
	}

	BOOL bSuccess = SetPixelFormat(_hDeviceContext, iPixelFormat, &pixelFormat);
	if ( !bSuccess )
	{
		ReleaseDC(hWindow, _hDeviceContext);
		return false;
	}

	_hWindowRC = wglCreateContext(_hDeviceContext);
	if ( !_hWindowRC )
	{
		ReleaseDC(hWindow, _hDeviceContext);
		return false;
	}
	bSuccess = wglMakeCurrent(_hDeviceContext, _hWindowRC);
	if ( !bSuccess )
	{
		wglDeleteContext(_hWindowRC);
		ReleaseDC(hWindow, _hDeviceContext);
		return false;
	}

	GLint err = glewInit();
	if(GLEW_OK != err)
	{
		MessageBox( hWindow, "Error initializing GLEW", "RENDERER ERROR", MB_OK | MB_ICONSTOP );
		return false;
	}

	resizeViewPort(iWidth,iHeight);
	return true;
}

bool GLRenderer::releaseContent()
{
	wglDeleteContext(_hWindowRC);
	ReleaseDC(_hWindow, _hDeviceContext);
	return true;
}

void GLRenderer::setClearColor(const ColorRGBA &color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void GLRenderer::clearBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::swapBuffer()
{
	SwapBuffers(_hDeviceContext);
}

void GLRenderer::resizeViewPort(int width, int height)
{
	Renderer::resizeViewPort(width, height);
	glViewport(0,0,width, height);
}