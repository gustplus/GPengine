#ifndef _SOFTWARE_VIEW_H_
#define _SOFTWARE_VIEW_H_

#include "RenderDefs.h"
#include "GPVector.h"
#include "types.h"
#include "glfunc.h"
#include "GPRenderer.h"
#include "GPGLShaderProgram.h"

namespace GPEngine3D{
	class SoftwareRenderer :
		public Renderer
	{
	public:
		SoftwareRenderer(PixelFormat pixelformat, char depthBits, char stencilBits, bool doubleBuffer, int width, int height);
		~SoftwareRenderer();

		bool initContent();

		void drawPoint(Point2i &v, Color3b & color);
		void drawPoint(int x, int y, Color3b & color);

		void drawLine(Point2i &v0, Point2i &v1, Color3b &color);
		void drawLine(int x0, int y0, int x1, int y1, Color3b &color);

		void drawTriangleSolid(Point2f &v0, Point2f &v1, Point2f &v2, Color3b &color);
		void drawTriangleSolid(int x0, int y0, int x1, int y1, int x2, int y2, Color3b &color);

		void clearBuffer();

		void setClearColor(const ColorRGBA &color) override;

		void resizeViewPort(int width, int height) override;

		void swapBuffer();

	private:
		byte *_bBuf;
		Color3b _clearColor;

		GLuint _colorBuffer;

		GLfloat *_screenRect;
	};
}

#endif