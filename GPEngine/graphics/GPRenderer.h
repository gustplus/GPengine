#ifndef _RENDER_
#define _RENDER_

#include "gpobject.h"
#include "GPColorRGBA.h"

namespace GPEngine3D{
    
	class Renderer :
		public Object
	{
	public:
		enum PixelFormat{
			FM_RGB,
			FM_RGBA
		};

		virtual ~Renderer(void);

		virtual bool InitContent(bool fullScreen) = 0;

		virtual bool ReleaseContent() = 0;

		int GetHeight(){return iHeight;}
		int GetWidth(){return iWidth;}

		void SetHeight(int value){iHeight = value;}
		void SetWidth(int value){iWidth = value;}

		int GetDepthBits(){return cDepthBits;}
		int GetStencilBits(){return cStencilBits;}
		int GetColorBits(){return color_bits;}

		virtual bool RequestFullScreen() = 0;

		virtual void SetClearColor(ColorRGBA& color){clearColor = color;}

		virtual void ClearBuffer() = 0;

		virtual void SwapBuffer() = 0;

		virtual void resizeViewPort(int width, int height);

	protected:
		Renderer(PixelFormat pFormat, char depthBits, char stencilBits, bool doubleBuffer, int width, int height);

		PixelFormat pFormat;
		char cDepthBits;
		char cStencilBits;
		bool bDoubleBuffer; 
		int color_bits;

		int iWidth;
		int iHeight;

		ColorRGBA clearColor;

	};

}
#endif