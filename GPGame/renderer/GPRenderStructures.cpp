#include "GPRenderStructures.h"
#include "GPMatrix4.h"
namespace GPEngine3D{

	RenderList::~RenderList()
	{
		// GP_SAFE_DELETE_ARRAY(triangleArray);
	}

	void RenderList::vertexAttribPosPointer(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 count)
	{
		int offset = 0;
		uint_32 v_size = size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			localList[index].p.x = vertices[offset++];
			localList[index].p.y = vertices[offset++];
			localList[index].p.z = vertices[offset++];
			localList[index].p.w = 1;
			offset += stride;
		}
		for(int_32 index = v_size; index < count; ++index)
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

	void RenderList::vertexAttribColorPointer(bool normalize, uint_32 stride, byte *const colors, uint_32 count)
	{
		int offset = 0;
		uint_32 v_size = size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			tranList[index].color.r = colors[offset++];
			tranList[index].color.g = colors[offset++];
			tranList[index].color.b = colors[offset++];
			tranList[index].color.a = colors[offset++];
			offset += stride;
		}
		for(int_32 index = v_size; index < count; ++index)
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
		_attr |= POLY_ATTR::ENABLE_COLOR;
	}

	void RenderList::vertexAttribTexCoordPointer(bool normalize, uint_32 stride, float *const texCoords, uint_32 count)
	{
		int offset = 0;
		uint_32 v_size = size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			tranList[index].uv.x = texCoords[offset++];
			tranList[index].uv.y = texCoords[offset++];
			offset += stride;
		}
		for(int_32 index = v_size; index < count; ++index)
		{
			Vertex v;
			v.uv.x = texCoords[offset++];
			v.uv.y = texCoords[offset++];
			localList.push_back(v);
			tranList.push_back(v);
			offset += stride;
		}
		_attr |= POLY_ATTR::ENABLE_TEX;
	}

	void RenderList::disable(POLY_ATTR attr)
	{
		_attr &= ~attr;
	}

	uint_32 RenderList::size()
	{
		return localList.size();
	}


	PolyObject::~PolyObject()
	{
		
	}

	void PolyObject::vertexAttribPosPointer(bool normalize, uint_32 stride, GLfloat *const vertices, uint_32 count)
	{
		int offset = 0;
		uint_32 v_size = size();
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

	void PolyObject::vertexAttribColorPointer(bool normalize, uint_32 stride, byte *const colors, uint_32 count)
	{
		int offset = 0;
		uint_32 v_size = size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			triangleArray[index].tranList[0].color.r = colors[offset++];
			triangleArray[index].tranList[0].color.g = colors[offset++];
			triangleArray[index].tranList[0].color.b = colors[offset++];
			triangleArray[index].tranList[0].color.a = colors[offset++];
			
			triangleArray[index].tranList[1].color.r = colors[offset++];
			triangleArray[index].tranList[1].color.g = colors[offset++];
			triangleArray[index].tranList[1].color.b = colors[offset++];
			triangleArray[index].tranList[1].color.a = colors[offset++];
			
			triangleArray[index].tranList[2].color.r = colors[offset++];
			triangleArray[index].tranList[2].color.g = colors[offset++];
			triangleArray[index].tranList[2].color.b = colors[offset++];
			triangleArray[index].tranList[2].color.a = colors[offset++];

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

		_attr |= POLY_ATTR::ENABLE_COLOR;
	}

	void PolyObject::vertexAttribTexCoordPointer(bool normalize, uint_32 stride, float *const texCoords, uint_32 count)
	{
		int offset = 0;
		uint_32 v_size = size();
		for(int_32 index = 0; index < v_size; ++index)
		{
			triangleArray[index].tranList[0].uv.x = texCoords[offset++];
			triangleArray[index].tranList[0].uv.y = texCoords[offset++];
			
			triangleArray[index].tranList[1].uv.x = texCoords[offset++];
			triangleArray[index].tranList[1].uv.y = texCoords[offset++];
			
			triangleArray[index].tranList[2].uv.x = texCoords[offset++];
			triangleArray[index].tranList[2].uv.y = texCoords[offset++];

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

		_attr |= POLY_ATTR::ENABLE_TEX;
	}

	void PolyObject::disable(POLY_ATTR attr)
	{
		_attr &= ~attr;
	}

	uint_32 PolyObject::size()
	{
		return triangleArray.size();
	}


	/*
	after this transform, the x, y, z param of each vertex will be in range[-1, 1],
	and w param will be -z
	*/
	void _cameraToProjectionTransform(PolyObject &buffer, uint_32 offset, const Matrix4 &projMat, uint_32 count)
	{
		for(int index = 0; index < count; ++index)
		{
			buffer.triangleArray[index].tranList[0].p = projMat * buffer.triangleArray[index].localList[0].p;
			buffer.triangleArray[index].tranList[1].p = projMat * buffer.triangleArray[index].localList[1].p;
			buffer.triangleArray[index].tranList[2].p = projMat * buffer.triangleArray[index].localList[2].p;
		}
	}

	void transformRenderList(RenderList &buffer, const Matrix4 &mat)
	{
		uint_32 size = buffer.size();
		for(int idx = 0; idx < size; ++idx)
		{
			buffer.tranList[idx].p = mat * buffer.localList[idx].p;
		}
	}
}