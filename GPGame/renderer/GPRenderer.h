#ifndef _RENDER_H_
#define _RENDER_H_

#include "gpobject.h"
#include "GPColorRGBA.h"
#include "RenderDefs.h"
#include "types.h"
//#include "GPSystem.h"

namespace GPEngine3D{
	class Renderer :
		public Object
	{
	public:
		virtual ~Renderer(void);

		int getHeight(){return iHeight;}
		int getWidth(){return iWidth;}

		void setHeight(int value){iHeight = value;}
		void setWidth(int value){iWidth = value;}

		int getDepthBits(){return cDepthBits;}
		int getStencilBits(){return cStencilBits;}
		int getColorBytes(){return colorBytes;}

		virtual void setClearColor(const ColorRGBA &color) = 0;

		virtual void clearBuffer() = 0;

		virtual void swapBuffer() = 0;

		virtual void resizeViewPort(int width, int height);

	protected:
		Renderer(PixelFormat pFormat, char depthBits, char stencilBits, bool doubleBuffer, int width, int height);

		PixelFormat pFormat;
		char cDepthBits;
		char cStencilBits;
		bool bDoubleBuffer; 
		int colorBytes;

		int iWidth;
		int iHeight;
	};

}
#endif