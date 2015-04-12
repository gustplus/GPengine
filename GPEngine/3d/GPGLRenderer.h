#ifndef _GLRENDER_
#define _GLRENDER_
#include "gprenderer.h"
#include <windows.h>

namespace GPEngine3D{

	class GLRenderer :
		public Renderer
	{
	public:
		GLRenderer(HWND window, PixelFormat pFormat, char depthBits, char stencilBits, bool doubleBuffer ,int width, int height);
		virtual ~GLRenderer(void);

		virtual bool InitContent(bool fullScreen);

		virtual bool ReleaseContent();

		virtual bool RequestFullScreen();

		virtual void ClearBuffer();

		virtual void SwapBuffer();

		//void SetClearColor(ColorRGBA& color){glClearColor(color.fR);}

		virtual void resizeViewPort(int width, int height);

	private:
		HWND m_Window;
		HDC deviceContext;
		HGLRC hWindowRC;
	};
}
#endif