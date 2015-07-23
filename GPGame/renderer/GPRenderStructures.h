#ifndef _RENDER_STRUCTURES_H_
#define _RENDER_STRUCTURES_H_

#include "GPVector.h"
#include "gp.h"
#include "types.h"
#include <vector>
#include "glfunc.h"
#include "GPMatrix4.h"
#include "GPSoftwareTexture.h"

#define CCW  1
#define CW  -1
#define NO_CULL  0

#define CullMode int

namespace GPEngine3D {
enum class PixelFormat{
	RGB,
	RGBA
};

typedef struct Color3b {
	union
	{
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
		};
		unsigned char data[3];
	};
} Color3b;

typedef struct Color4b {
	union
	{
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};
		unsigned char data[4];
	};
} Color4b;

enum ATTR_FLAG
{
	COLOR_BUFFER = 0x1,
	DEPTH_BUFFER = 0x2,
	STENCIL_BUFFER = 0x4
};

typedef struct
{
	vec4f p;
	Color4b color;
	vec2f uv;
	vec3f normal;
	int normalLen;
}Vertex;

typedef struct
{
	Vertex localList[3];
	Vertex tranList[3];

	vec3f normal;
	int normalLen;
	int attr;
}PolyTriangle;

enum POLY_ATTR{
	NONE = 0x0,
	ENABLE_COLOR = 0x1,
	ENABLE_TEX = 0x2
};

class RenderList
{
public:
	std::vector<Vertex> localList;
	std::vector<Vertex> tranList;

	RenderList():_attr(POLY_ATTR::NONE), _tex(nullptr){};
	~RenderList();

	void vertexAttribPosPointer(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 count);
	void vertexAttribColorPointer(bool normalize, uint_32 stride, byte *const colors, uint_32 count);
	void vertexAttribTexCoordPointer(bool normalize, uint_32 stride, float *const texCoords, uint_32 count);
	void setTexture(Texture2D *img){_tex = img;};
	Texture2D *getTexture(){return _tex;};
	void enable(POLY_ATTR attr) {_attr |= attr;};
	void disable(POLY_ATTR attr);
	bool isEnabled(POLY_ATTR attr){return (_attr & attr) > 0;};
	uint_32 size();
private:
	uint_32 _attr;
	Texture2D *_tex;
};

class PolyObject
{
public:
	std::vector<PolyTriangle> triangleArray;

	PolyObject():_attr(POLY_ATTR::NONE), _tex(nullptr){};

	~PolyObject();

	void vertexAttribPosPointer(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 count);
	void vertexAttribColorPointer(bool normalize, uint_32 stride, byte *const colors, uint_32 count);
	void vertexAttribTexCoordPointer(bool normalize, uint_32 stride, float *const texCoords, uint_32 count);
	void setTexture(Texture2D *img){_tex = img;};
	Texture2D *getTexture(){return _tex;};
	void enable(POLY_ATTR attr) {_attr |= attr;};
	void disable(POLY_ATTR attr);
	bool isEnabled(POLY_ATTR attr){return (_attr & attr) > 0;};
	uint_32 size();
private:
	uint_32 _attr;
	Texture2D *_tex;
};

void transformPolygen(PolyObject &buffer, uint_32 offset, const Matrix4 &projMat, uint_32 count);

void transformRenderList(RenderList &buffer, const Matrix4 &projMat);

bool isFaceVisible(const vec3f &p0, const vec3f &p1, const vec3f &p2, CullMode mode);

}
#endif