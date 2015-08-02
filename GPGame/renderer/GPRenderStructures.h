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

#define CLIP_LX 0x1
#define CLIP_RX 0x2
#define CLIP_IX 0x4
#define CLIP_BY 0x8
#define CLIP_TY 0x10
#define CLIP_IY 0x20
#define CLIP_NZ 0x40
#define CLIP_FZ 0x80
#define CLIP_IZ 0x100

#define RESET_VISIBLE_FLAG 0x0ff

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

enum POLY_ATTR{
	NONE = 0x0,
	ENABLE_COLOR = 0x1,
	ENABLE_TEX = 0x2,
	
	FLAT_SHADE = 0x10,
	SMOOTH_SHADE = 0x20,

	CULLED = 0x100
};



typedef struct
{
	uint_16 indices[3];
	vec3f normal;
	int_32 normalLen;
	uint_32 attr;
} Polygon, *PolyPtr;

class PolyObject
{
public:
	std::vector<Vertex> localList;
	std::vector<Vertex> tranList;

	std::vector<Polygon> polygens;

	PolyObject():_attr(POLY_ATTR::NONE), _tex(nullptr){};
	~PolyObject();

	void vertexAttribPosPointer(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 vertexCount);
	void vertexAttribColorPointer(bool normalize, uint_32 stride, byte *const colors, uint_32 vertexCount);
	void vertexAttribTexCoordPointer(bool normalize, uint_32 stride, float *const texCoords, uint_32 vertexCount);
	void vertexAttribIndexPointer(uint_32 stride, uint_16 *const indices, uint_32 indexCount);
	void setTexture(Texture2D *img){_tex = img;};
	Texture2D *getTexture(){return _tex;};
	uint_32 getAttrCode(){return _attr;};
	void setAttrCode(uint_32 attr){_attr = attr;};
	void enable(POLY_ATTR attr) {_attr |= attr;};
	void disable(POLY_ATTR attr);
	bool isEnabled(POLY_ATTR attr){return (_attr & attr) > 0;};
	uint_32 indexCount();
	uint_32 vertexCount();
private:
	uint_32 _attr;
	Texture2D *_tex;

	uint_32 _vertexCount;
	uint_32 _indexCount;
};

typedef struct
{
	Vertex localList[3];
	Vertex tranList[3];

	vec3f normal;
	int normalLen;
	uint_32 attr;
}PolyTriangle;

class RenderList
{
public:
	std::vector<PolyTriangle> triangleArray;

	RenderList():_attr(POLY_ATTR::NONE), _tex(nullptr), _triangleCount(0){};

	~RenderList();

	void reset() {_triangleCount = 0;};

	void vertexAttribPosPointer(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 faceCount);
	void vertexAttribColorPointer(bool normalize, uint_32 stride, byte *const colors, uint_32 faceCount);
	void vertexAttribTexCoordPointer(bool normalize, uint_32 stride, float *const texCoords, uint_32 faceCount);

	void insertTrianglePolygen(const Vertex &v0, const Vertex &v1, const Vertex &v2, const vec3f &normal, uint_32 attr, int normalLen = 0);

	void insertVertexAttribPos(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 faceCount);
	void insertVertexAttribColor(bool normalize, uint_32 stride, byte *const colors, uint_32 faceCount);
	void insertVertexAttribTexCoord(bool normalize, uint_32 stride, float *const texCoords, uint_32 faceCount);
	void setTexture(Texture2D *img){_tex = img;};

	uint_32 getAttrCode(){return _attr;};
	void setAttrCode(uint_32 attr){_attr = attr;};

	Texture2D *getTexture(){return _tex;};
	void enable(POLY_ATTR attr) {_attr |= attr;};
	void disable(POLY_ATTR attr);
	bool isEnabled(POLY_ATTR attr){return (_attr & attr) > 0;};
	uint_32 size();
private:
	uint_32 _attr;
	Texture2D *_tex;

	uint_32 _triangleCount;
};

void transformRenderList(RenderList &buffer, uint_32 offset, const Matrix4 &mat, uint_32 count);

void transformPolyObject(PolyObject &buffer, const Matrix4 &mat);

bool isFaceVisible(const vec3f &p0, const vec3f &p1, const vec3f &p2, CullMode mode);

void setBit(uint_32 &attr, int flag);

}
#endif