#ifndef _SOFTWARE_VIEW_H_
#define _SOFTWARE_VIEW_H_

#include "GPVector.h"
#include "types.h"
#include "glfunc.h"
#include "GPRenderer.h"
#include "GPRenderStructures.h"
#include "GPMatrix4.h"
#include "GPSoftwareTexture.h"

namespace GPEngine3D{
	class SoftwareRenderer :
		public Renderer
	{
	public:
		SoftwareRenderer(PixelFormat pixelformat, char depthBits, char stencilBits, bool doubleBuffer, int width, int height);
		~SoftwareRenderer();

		bool initContent();
		void resizeViewPort(int width, int height) override;
		void swapBuffer();

		void drawPoint(const Point2i &v, const Color3b & color);
		void drawPoint(int x, int y, const Color3b & color);

		void drawLine(const Point2i &v0, const Point2i &v1, const Color3b &color);
		void drawLine(int x0, int y0, int x1, int y1, const Color3b &color);

		void drawTriangleSolidFlat(const PolyTriangle &triangle);
		void drawTriangleSolidFlat(const Vertex &v0, const Vertex & v1, const Vertex &v2);
		void drawTriangleSolidColor(const PolyTriangle &triangle);
		void drawTriangleSolidColor(const Vertex &v0, const Vertex & v1, const Vertex &v2);
		void drawTriangleSolidTexNearest(const PolyTriangle &triangle, const Texture2D &tex);
		void drawTriangleSolidTexNearest(const Vertex &v0, const Vertex & v1, const Vertex &v2, const Texture2D &tex);
		void drawTriangleSolidTexLinear(const PolyTriangle &triangle, const Texture2D &tex);
		void drawTriangleSolidTexLinear(const Vertex &v0, const Vertex & v1, const Vertex &v2, const Texture2D &tex);
		void drawTriangleSolidColorTex(const PolyTriangle &triangle, const Texture2D &tex);
		void drawTriangleSolidColorTex(const Vertex &v0, const Vertex & v1, const Vertex &v2, const Texture2D &tex);

		void clearBuffer();
		void clearBuffer(ATTR_FLAG flags);
		void setClearColor(const ColorRGBA &color) override;

		void drawArray(PolyObject &buffer, uint_32 offset,  uint_32 faceCount);
		void drawElements(RenderList &buffer, GLushort *const indices, uint_32 count);

		void _clearColorBuffer();
		void _clearDepthBuffer();
		
		//I decide to take these two function out of draw functions, because if the model if no change, there is no need to recalculate
		void projectionToScreenTransform(PolyObject &buffer, uint_32 offset, uint_32 count);
		void projectionToScreenTransform(RenderList &buffer);

		void enable(ATTR_FLAG flag);
		void disable(ATTR_FLAG flag);
		// static const int_32 ATTR__BUFFER = 0x4;

	private:
		byte *_bColorBuffer;
		float *_fDepthBuffer;

		Color3b _clearColor;

		//int iWidth;
		//int iHeight;

		int iLeftX;
		int iBottomY;

		int iRightX;
		int iTopY;

		int_32 _attr;
	};
}

#endif