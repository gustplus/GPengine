#include "GPRenderStructures.h"
#include "GPMatrix4.h"
namespace GPEngine3D{

	PolyObject::~PolyObject()
	{
	}

	void PolyObject::vertexAttribPosPointer(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 vertexCount)
	{
		_attr &= RESET_VISIBLE_FLAG;
		_vertexCount = vertexCount;
		int offset = 0;
		uint_32 v_size = localList.size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			localList[index].p.x = vertices[offset];
			tranList[index].p.x = vertices[offset++];
			localList[index].p.y = vertices[offset];
			tranList[index].p.y = vertices[offset++];
			localList[index].p.z = vertices[offset];
			tranList[index].p.z = vertices[offset++];
			localList[index].p.w = 1;
			tranList[index].p.w = 1;
			offset += stride;
		}
		for(int_32 index = v_size; index < vertexCount; ++index)
		{
			Vertex v;
			v.p.x = vertices[offset++];
			v.p.y = vertices[offset++];
			v.p.z = vertices[offset++];
			v.p.w = 1;
			localList.push_back(v);
			tranList.push_back(v);
			offset += stride;
		}
	}

	void PolyObject::vertexAttribColorPointer(bool normalize, uint_32 stride, byte *const colors, uint_32 vertexCount)
	{
		int offset = 0;
		uint_32 v_size = localList.size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			localList[index].color.r = colors[offset++];
			localList[index].color.g = colors[offset++];
			localList[index].color.b = colors[offset++];
			localList[index].color.a = colors[offset++];
			offset += stride;
		}
		for(int_32 index = v_size; index < vertexCount; ++index)
		{
			Vertex v;
			v.color.r = colors[offset++];
			v.color.g = colors[offset++];
			v.color.b = colors[offset++];
			v.color.a = colors[offset++];
			localList.push_back(v);
			tranList.push_back(v);
			offset += stride;
		}
		setBit(_attr, POLY_ATTR::ENABLE_COLOR);
	}

	void PolyObject::vertexAttribTexCoordPointer(bool normalize, uint_32 stride, float *const texCoords, uint_32 vertexCount)
	{
		int offset = 0;
		uint_32 v_size = localList.size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			localList[index].uv.x = texCoords[offset++];
			localList[index].uv.y = texCoords[offset++];
			offset += stride;
		}
		for(int_32 index = v_size; index < vertexCount; ++index)
		{
			Vertex v;
			v.uv.x = texCoords[offset++];
			v.uv.y = texCoords[offset++];
			localList.push_back(v);
			tranList.push_back(v);
			offset += stride;
		}
		setBit(_attr, POLY_ATTR::ENABLE_TEX);
	}

	void PolyObject::vertexAttribIndexPointer(uint_32 stride, uint_16 *const indices, uint_32 indexCount)
	{
		_indexCount = indexCount;
		int offset = 0;
		uint_32 v_size = polygens.size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			polygens[index].attr &= RESET_VISIBLE_FLAG;
			polygens[index].indices[0] = indices[offset++];
			polygens[index].indices[1] = indices[offset++];
			polygens[index].indices[2] = indices[offset++];
			offset += stride;
		}

		for(int_32 index = v_size; index < indexCount; ++index)
		{
			Polygon polygen;
			polygen.attr &= RESET_VISIBLE_FLAG;
			polygen.indices[0] = indices[offset++];
			polygen.indices[1] = indices[offset++];
			polygen.indices[2] = indices[offset++];
			polygens.push_back(polygen);
			offset += stride;
		}
	}

	void PolyObject::disable(POLY_ATTR attr)
	{
		_attr &= ~attr;
	}

	uint_32 PolyObject::vertexCount()
	{
		return _vertexCount;
	}

	uint_32 PolyObject::indexCount()
	{
		return _indexCount;
	}


	RenderList::~RenderList()
	{
		
	}

	void RenderList::vertexAttribPosPointer(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 count)
	{
		int offset = 0;
		uint_32 v_size = triangleArray.size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			triangleArray[index].localList[0].p.x = vertices[offset++];
			triangleArray[index].localList[0].p.y = vertices[offset++];
			triangleArray[index].localList[0].p.z = vertices[offset++];
			triangleArray[index].localList[0].p.w = 1;
			
			triangleArray[index].localList[1].p.x = vertices[offset++];
			triangleArray[index].localList[1].p.y = vertices[offset++];
			triangleArray[index].localList[1].p.z = vertices[offset++];
			triangleArray[index].localList[1].p.w = 1;
			
			triangleArray[index].localList[2].p.x = vertices[offset++];
			triangleArray[index].localList[2].p.y = vertices[offset++];
			triangleArray[index].localList[2].p.z = vertices[offset++];
			triangleArray[index].localList[2].p.w = 1;

			offset += stride;
		}

		for(int_32 index = v_size; index < count; ++index)
		{
			PolyTriangle triangle;
			triangle.localList[0].p.x = vertices[offset++];
			triangle.localList[0].p.y = vertices[offset++];
			triangle.localList[0].p.z = vertices[offset++];
			triangle.localList[0].p.w = 1;
			
			triangle.localList[1].p.x = vertices[offset++];
			triangle.localList[1].p.y = vertices[offset++];
			triangle.localList[1].p.z = vertices[offset++];
			triangle.localList[1].p.w = 1;
			
			triangle.localList[2].p.x = vertices[offset++];
			triangle.localList[2].p.y = vertices[offset++];
			triangle.localList[2].p.z = vertices[offset++];
			triangle.localList[2].p.w = 1;
			
			triangleArray.push_back(triangle);

			offset += stride;
		}
	}

	void RenderList::vertexAttribColorPointer(bool normalize, uint_32 stride, byte *const colors, uint_32 count)
	{
		int offset = 0;
		uint_32 v_size = triangleArray.size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			triangleArray[index].localList[0].color.r = colors[offset++];
			triangleArray[index].localList[0].color.g = colors[offset++];
			triangleArray[index].localList[0].color.b = colors[offset++];
			triangleArray[index].localList[0].color.a = colors[offset++];
			
			triangleArray[index].localList[1].color.r = colors[offset++];
			triangleArray[index].localList[1].color.g = colors[offset++];
			triangleArray[index].localList[1].color.b = colors[offset++];
			triangleArray[index].localList[1].color.a = colors[offset++];
			
			triangleArray[index].localList[2].color.r = colors[offset++];
			triangleArray[index].localList[2].color.g = colors[offset++];
			triangleArray[index].localList[2].color.b = colors[offset++];
			triangleArray[index].localList[2].color.a = colors[offset++];

			offset += stride;
		}

		for(int_32 index = v_size; index < count; ++index)
		{
			PolyTriangle triangle;

			triangle.localList[0].color.r = colors[offset++];
			triangle.localList[0].color.g = colors[offset++];
			triangle.localList[0].color.b = colors[offset++];
			triangle.localList[0].color.a = colors[offset++];

			triangle.localList[1].color.r = colors[offset++];
			triangle.localList[1].color.g = colors[offset++];
			triangle.localList[1].color.b = colors[offset++];
			triangle.localList[1].color.a = colors[offset++];

			triangle.localList[2].color.r = colors[offset++];
			triangle.localList[2].color.g = colors[offset++];
			triangle.localList[2].color.b = colors[offset++];
			triangle.localList[2].color.a = colors[offset++];

			triangleArray.push_back(triangle);

			offset += stride;
		}

		setBit(_attr, POLY_ATTR::ENABLE_COLOR);
	}

	void RenderList::vertexAttribTexCoordPointer(bool normalize, uint_32 stride, float *const texCoords, uint_32 count)
	{
		int offset = 0;
		uint_32 v_size = triangleArray.size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			triangleArray[index].localList[0].uv.x = texCoords[offset++];
			triangleArray[index].localList[0].uv.y = texCoords[offset++];
			
			triangleArray[index].localList[1].uv.x = texCoords[offset++];
			triangleArray[index].localList[1].uv.y = texCoords[offset++];
			
			triangleArray[index].localList[2].uv.x = texCoords[offset++];
			triangleArray[index].localList[2].uv.y = texCoords[offset++];

			offset += stride;
		}

		for(int_32 index = v_size; index < count; ++index)
		{
			PolyTriangle triangle;

			triangle.localList[0].uv.x = texCoords[offset++];
			triangle.localList[0].uv.y = texCoords[offset++];

			triangle.localList[1].uv.x = texCoords[offset++];
			triangle.localList[1].uv.y = texCoords[offset++];

			triangle.localList[2].uv.x = texCoords[offset++];
			triangle.localList[2].uv.y = texCoords[offset++];

			triangleArray.push_back(triangle);

			offset += stride;
		}

		setBit(_attr, POLY_ATTR::ENABLE_TEX);
	}

	void RenderList::insertTrianglePolygen(const Vertex &v0, const Vertex &v1, const Vertex &v2, const vec3f &normal, uint_32 attr, int normalLen)
	{
		PolyTriangle triangle;
		triangle.tranList[0] = v0;
		triangle.tranList[1] = v1;
		triangle.tranList[2] = v2;
		triangle.normal = normal;
		triangle.attr = attr;
		triangle.normalLen = normalLen;
		if(triangleArray.size() <= _triangleCount)
		{
			triangleArray.push_back(triangle);	
		}else{
			triangleArray[_triangleCount] = triangle;
		}
		++_triangleCount;
	}

	void RenderList::insertVertexAttribPos(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 count)
	{
		int startIdx = _triangleCount;
		int offset = 0;
		uint_32 v_size = triangleArray.size();
		for(int_32 index = startIdx; index < v_size; ++index)
		{
			triangleArray[index].localList[0].p.x = vertices[offset++];
			triangleArray[index].localList[0].p.y = vertices[offset++];
			triangleArray[index].localList[0].p.z = vertices[offset++];
			triangleArray[index].localList[0].p.w = 1;
			
			triangleArray[index].localList[1].p.x = vertices[offset++];
			triangleArray[index].localList[1].p.y = vertices[offset++];
			triangleArray[index].localList[1].p.z = vertices[offset++];
			triangleArray[index].localList[1].p.w = 1;
			
			triangleArray[index].localList[2].p.x = vertices[offset++];
			triangleArray[index].localList[2].p.y = vertices[offset++];
			triangleArray[index].localList[2].p.z = vertices[offset++];
			triangleArray[index].localList[2].p.w = 1;

			offset += stride;
		}

		for(int_32 index = v_size; index < count; ++index)
		{
			PolyTriangle triangle;
			triangle.localList[0].p.x = vertices[offset++];
			triangle.localList[0].p.y = vertices[offset++];
			triangle.localList[0].p.z = vertices[offset++];
			triangle.localList[0].p.w = 1;
			
			triangle.localList[1].p.x = vertices[offset++];
			triangle.localList[1].p.y = vertices[offset++];
			triangle.localList[1].p.z = vertices[offset++];
			triangle.localList[1].p.w = 1;
			
			triangle.localList[2].p.x = vertices[offset++];
			triangle.localList[2].p.y = vertices[offset++];
			triangle.localList[2].p.z = vertices[offset++];
			triangle.localList[2].p.w = 1;
			
			triangleArray.push_back(triangle);

			offset += stride;
		}
	}

	void RenderList::insertVertexAttribColor(bool normalize, uint_32 stride, byte *const colors, uint_32 count)
	{
		int startIdx = _triangleCount;
		int offset = 0;
		uint_32 v_size = triangleArray.size();
		for(int_32 index = startIdx; index < v_size; ++index)
		{
			triangleArray[index].localList[0].color.r = colors[offset++];
			triangleArray[index].localList[0].color.g = colors[offset++];
			triangleArray[index].localList[0].color.b = colors[offset++];
			triangleArray[index].localList[0].color.a = colors[offset++];
			
			triangleArray[index].localList[1].color.r = colors[offset++];
			triangleArray[index].localList[1].color.g = colors[offset++];
			triangleArray[index].localList[1].color.b = colors[offset++];
			triangleArray[index].localList[1].color.a = colors[offset++];
			
			triangleArray[index].localList[2].color.r = colors[offset++];
			triangleArray[index].localList[2].color.g = colors[offset++];
			triangleArray[index].localList[2].color.b = colors[offset++];
			triangleArray[index].localList[2].color.a = colors[offset++];

			offset += stride;
		}

		for(int_32 index = v_size; index < count; ++index)
		{
			PolyTriangle triangle;

			triangle.localList[0].color.r = colors[offset++];
			triangle.localList[0].color.g = colors[offset++];
			triangle.localList[0].color.b = colors[offset++];
			triangle.localList[0].color.a = colors[offset++];

			triangle.localList[1].color.r = colors[offset++];
			triangle.localList[1].color.g = colors[offset++];
			triangle.localList[1].color.b = colors[offset++];
			triangle.localList[1].color.a = colors[offset++];

			triangle.localList[2].color.r = colors[offset++];
			triangle.localList[2].color.g = colors[offset++];
			triangle.localList[2].color.b = colors[offset++];
			triangle.localList[2].color.a = colors[offset++];

			triangleArray.push_back(triangle);

			offset += stride;
		}

		setBit(_attr, POLY_ATTR::ENABLE_COLOR);
	}

	void RenderList::insertVertexAttribTexCoord(bool normalize, uint_32 stride, float *const texCoords, uint_32 count)
	{
		int startIdx = _triangleCount;
		int offset = 0;
		uint_32 v_size = triangleArray.size();
		for(int_32 index = startIdx; index < v_size; ++index)
		{
			triangleArray[index].localList[0].uv.x = texCoords[offset++];
			triangleArray[index].localList[0].uv.y = texCoords[offset++];
			
			triangleArray[index].localList[1].uv.x = texCoords[offset++];
			triangleArray[index].localList[1].uv.y = texCoords[offset++];
			
			triangleArray[index].localList[2].uv.x = texCoords[offset++];
			triangleArray[index].localList[2].uv.y = texCoords[offset++];

			offset += stride;
		}

		for(int_32 index = v_size; index < count; ++index)
		{
			PolyTriangle triangle;

			triangle.localList[0].uv.x = texCoords[offset++];
			triangle.localList[0].uv.y = texCoords[offset++];

			triangle.localList[1].uv.x = texCoords[offset++];
			triangle.localList[1].uv.y = texCoords[offset++];

			triangle.localList[2].uv.x = texCoords[offset++];
			triangle.localList[2].uv.y = texCoords[offset++];

			triangleArray.push_back(triangle);

			offset += stride;
		}

		setBit(_attr, POLY_ATTR::ENABLE_TEX);
	}

	void RenderList::disable(POLY_ATTR attr)
	{
		_attr &= ~attr;
	}

	uint_32 RenderList::size()
	{
		return _triangleCount;
	}

	void transformRenderList(RenderList &buffer, uint_32 offset, const Matrix4 &mat, uint_32 count)
	{
		for(int index = 0; index < count; ++index)
		{
			buffer.triangleArray[index].tranList[0].p = mat * buffer.triangleArray[index].localList[0].p;
			buffer.triangleArray[index].tranList[1].p = mat * buffer.triangleArray[index].localList[1].p;
			buffer.triangleArray[index].tranList[2].p = mat * buffer.triangleArray[index].localList[2].p;
		}
	}

	void transformPolyObject(PolyObject &buffer, const Matrix4 &mat)
	{
		uint_32 size = buffer.vertexCount();
		for(int idx = 0; idx < size; ++idx)
		{
			buffer.tranList[idx].p = mat * buffer.localList[idx].p;
		}
	}

	bool isFaceVisible(const vec3f &p0, const vec3f &p1, const vec3f &p2, CullMode mode)
	{
		if(NO_CULL == mode)
		{
			return false;
		}
		vec3f v0(p0, p1);
		vec3f v1(p1, p2);
		vec3f normal = v0.crossMul(v1);
		float dot = p0.dotMul(normal);
		return (dot * mode) > 0;
	}

	void setBit(uint_32 &attr, int flag)
	{
		attr |= flag;
	}
}