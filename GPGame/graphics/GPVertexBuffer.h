#ifndef __GPEngine__GPVertexBuffer__
#define __GPEngine__GPVertexBuffer__

#include "gp.h"
#include "glfunc.h"
#include "types.h"

/*
 *you can use VertexBuffer to store vertex data, include vertex position, colors, and user defined datas
 *
 */

namespace GPEngine3D
{
    class VertexBufferBase :
    public Object
    {
    public:
        VertexBufferBase(int num_vertices, int num_per_vertex, int byte_per_data_cell, DATATYPE data_type, bool isDynamic = false):
        _num(num_vertices),
        _VBO(0),
        _isDynamic(isDynamic),
        _nums_per_vertex(num_per_vertex),
        _byte_per_data_cell(byte_per_data_cell),
        _location(-1){}
        
        virtual ~VertexBufferBase(){
            if (_VBO != 0){
                glDeleteBuffers(1, &_VBO);
            }
        }
        
        int getBindedLocation()
        {
            return _location;
        }
        
        virtual bool bindToAttribLocation(GLuint location) = 0;
        
        virtual bool bind()
        {
			if(_VBO != 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, _VBO);
				glVertexAttribPointer(_location, _num, _dataType, false, 0, BUFFER_OFFSET_BYTE(0));
				glEnableVertexAttribArray(_location);
				return true;
			}
            return false;
        }
        
        int getVerticeNum()
        {
            return _num;
        }
        
    protected:
        int _num;
        int _nums_per_vertex;
        DATATYPE _dataType;
        GLuint _VBO;
        GLint _location;
        
        byte _byte_per_data_cell;
        bool _isDynamic;
        
    };
    
    template<class T, int num_per_vertex>
	class VertexBuffer :
		public VertexBufferBase
	{
	public:
        VertexBuffer(T *vertices, int num_vertices, DATATYPE data_type, bool isDynamic = false)
        :VertexBufferBase(num_vertices, num_per_vertex, sizeof(T), data_type, isDynamic)
#ifdef GLES3
        ,
        _isMapingData(false)
#endif
        {
            genArrayBuffer(vertices);
        }
        
		virtual ~VertexBuffer(void)
        {
        if (_VBO != 0){
            glDeleteBuffers(1, &_VBO);
        }
        }

		bool genArrayBuffer(const T *vertices)
        {
			glGenBuffers(1, &_VBO);
			if(_VBO == 0)
			{
				return false;
			}
        
			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferData(GL_ARRAY_BUFFER, _num * _byte_per_data_cell, vertices, _isDynamic ? VBO_DYNAMIC_TYPE : VBO_STATIC_TYPE);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			_isDirty = true;
			return true;
        }

		bool bindToAttribLocation(int location)
        {
            if (_location < 1){
                return false;
            }
            _location = location;
            return true;
        }
        
        void changeVertexDataAt(uint_32 index, const T *data)
        {
            if (index < _num)
            {
                this->bind();
                glBufferSubData(GL_ARRAY_BUFFER, index * _nums_per_vertex * _byte_per_data_cell, _byte_per_data_cell, data);
            }else{
                printf("index is invalid at %s, %d", __FILE__, __LINE__);
            }
        }
        
        void changeVertexDataWithRange(uint_32 startIndex, uint_32 length, const T *data)
        {
            if (startIndex >= _num)
            {
                printf("index is invalid at %s, %d", __FILE__, __LINE__);
            }
            if (startIndex + length >= _num)
            {
                printf("length is invalid at %s, %d", __FILE__, __LINE__);
            }
            this->bind();
            glBufferSubData(GL_ARRAY_BUFFER, startIndex * _nums_per_vertex * _byte_per_data_cell, length * _byte_per_data_cell, data);
        }
        
#ifdef GLES3
        float *mapData(uint_32 start, int length, bool isRead)
        {
            if (_isMapingData)
            {
                printf("data has been mapped, please unmap it first");
                return NULL;
            }
            _isMapingData = true;
            return (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, start * _nums_per_vertex * _byte_per_data_cell, length * _nums_per_vertex, isRead ? GL_MAP_READ_BIT : GL_MAP_WRITE_BIT);
        }
        
        bool unmapData()
        {
            if (!_isMapingData)
            {
                printf("you hasn't map the data");
                return false;
            }
            _isMapingData = false;
            return glUnmapBuffer(GL_ARRAY_BUFFER);
        }
#endif
	private:
#ifdef GLES3
        bool _isMapingData;
#endif
	};


    
    
    DEFINE_SMARTPTR(VertexBufferBase, VertexBufferPtr);
    
    typedef VertexBuffer<float, 3>  PositionBuffer3f;
    DEFINE_SMARTPTR(PositionBuffer3f, PositionBuffer3fPtr);
    typedef VertexBuffer<float, 2> TexCoordsBuffer2f;
    DEFINE_SMARTPTR(TexCoordsBuffer2f, TexCoordsBuffer2fPtr);
    typedef VertexBuffer<float, 3>  NormalBuffer3f;
    DEFINE_SMARTPTR(NormalBuffer3f, NormalBuffer3fPtr);
    
    typedef VertexBuffer<float, 3>  VertexBuffer3f;
    DEFINE_SMARTPTR(VertexBuffer3f, VertexBuffer3fPtr);
}

#endif