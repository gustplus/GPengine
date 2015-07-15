#ifndef _SOFTWARE_VIEW_H_
#define _SOFTWARE_VIEW_H_

#include "GPVector.h"
#include "types.h"
#include "glfunc.h"
#include "GPRenderer.h"
#include "GPRenderStructures.h"
#include "GPMatrix4.h"

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

		void drawTriangleSolid(PolyTriangle *triangle);
		void drawTriangleSolid(const Vertex &v0, const Vertex & v1, const Vertex &v2);
		void drawTriangleSolid(const Point2f &v0, const Point2f &v1, const Point2f &v2, const Color3b &color);
		void drawTriangleSolid(int x0, int y0, int x1, int y1, int x2, int y2, const Color3b &color);

		void clearBuffer();
		void clearBuffer(ATTR_FLAG flags);
		void setClearColor(const ColorRGBA &color) override;

		void vertexAttribPosPointer(PolyObject *buffer, bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 count);
		void vertexAttribColorPointer(PolyObject *buffer, bool normalize, uint_32 stride, byte *const colors, uint_32 count);

		void drawArray(PolyObject *buffer, uint_32 offset, const Matrix4 &mat, uint_32 count);
		void drawElements(RenderList *buffer, uint_32 offset, GLushort *const indices, const Matrix4 &mat, uint_32 count);

		void _clearColorBuffer();
		void _clearDepthBuffer();

		//bool _localToWorldTransForm();
		void _cameraToProjectionTransform(PolyObject *buffer, uint_32 offset, const Matrix4 &projMat, uint_32 count);
		void _ProjectionToScreenTransform(PolyObject *buffer, uint_32 offset, uint_32 count);

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