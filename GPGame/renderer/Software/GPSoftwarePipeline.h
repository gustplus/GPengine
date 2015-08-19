#ifndef _SOFTWARE_RENDERER_H_
#define _SOFTWARE_RENDERER_H_

#include "GPVector.h"
#include "types.h"
#include "glfunc.h"
#include "GPRenderer.h"
#include "GPRenderStructures.h"
#include "GPMatrix4.h"
#include "GPSoftwareTexture.h"
#include "GPGLShaderProgram.h"
#include "stdafx.h"

namespace GPEngine3D{
	class SoftwarePipeline :
		public Renderer
	{
	public:
		SoftwarePipeline(PixelFormat pixelformat, char depthBits, char stencilBits, bool doubleBuffer, int width, int height);
		~SoftwarePipeline();

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

		void drawElements(PolyObject &buffer, uint_32 offset,  uint_32 faceCount);
		void drawArray(RenderList &buffer);

		void _clearColorBuffer();
		void _clearDepthBuffer();

		void cullFace(CullMode mode){_cullMode = mode;}

		void removeBackFaces(PolyObject &buffer, uint_32 offset, uint_32 faceCount);
		void removeBackFaces(RenderList &buffer, uint_32 offset, uint_32 faceCount);

		void insertObjectToRenderList(RenderList &list, PolyObject &obj, uint_32 offset, uint_32 faceCount);

		// CVV: Canonical View Volume
		void CVVClip(PolyObject &buffer, uint_32 offset, uint_32 faceCount);
		void CVVClip(RenderList &buffer);

		void nearFarPlaneClip(RenderList &buffer, float nearZ, float farZ);
		
		//I decide to take these two function out of draw functions, because if the model isn't change, there is no need to recalculate
		void projectionToScreenTransform(RenderList &buffer);
		void projectionToScreenTransform(PolyObject &buffer);

		void enable(ATTR_FLAG flag);
		void disable(ATTR_FLAG flag);

	protected:
		byte *_bColorBuffer;
		float *_fDepthBuffer;

		Color3b _clearColor;
		CullMode _cullMode;

		int iLeftX;
		int iBottomY;

		int iRightX;
		int iTopY;

		int_32 _attr;

		GLuint _screenRect;

		GLuint _screenTex; 
		GLuint _texIdx;

		GLShaderProgram _program;
		GLuint vbos[3];

		Matrix4 mv;

		GLuint uniformMat;
	};

	extern float invValues[12];
}

#endif