#ifndef _GL_FUNC_H_
#define _GL_FUNC_H_

//#define GP_PLATFORM_IOS
//#define GLES3

#ifdef GP_PLATFORM_IOS

#ifdef GLES3
#import "OpenGLES/ES3/gl.h"
#import "OpenGLES/ES3/glext.h"
#else
#import "OpenGLES/ES2/gl.h"
#import "OpenGLES/ES2/glext.h"
#endif

#endif

#ifdef GP_PLATFORM_WIN32
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#pragma comment(lib, "glew32.lib")
#endif

#include <iostream>
#include "stdafx.h"

typedef enum{
    VERTEX_SHADER = GL_VERTEX_SHADER,
    FRAG_SHADER = GL_FRAGMENT_SHADER,
    GEOMETRY_SHADER,
    TESS_CONTROL_SHADER,
    TESS_EVALUATION,
    NUM_OF_SHADER = 5
} ShaderType;

typedef enum{
    ONE = GL_ONE,
    ZERO = GL_ZERO,
    SRC_ALPHA = GL_SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
    ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
    SRC_COLOR = GL_SRC_COLOR,
    DST_ALPHA = GL_DST_ALPHA,
    ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
    DST_COLOR = GL_DST_COLOR,
    ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR
} BlendFactor;

typedef enum{
    VBO_STATIC_TYPE  = GL_STATIC_DRAW,
    VBO_DYNAMIC_TYPE = GL_DYNAMIC_DRAW
} VBO_DRAW_TYPE;

typedef enum{
    DT_BYTE = GL_UNSIGNED_BYTE,
    DT_INT = GL_INT,
    DT_FLOAT = GL_FLOAT,
    DT_UNSIGNED_SHORT = GL_UNSIGNED_SHORT
} DATATYPE;

typedef enum{
    DT_POINT = GL_POINTS,
    DT_LINE = GL_LINES,
    DT_LINE_STRIPE = GL_LINE_STRIP,
    DT_LINE_LOOP = GL_LINE_LOOP,
    DT_TRIANGLE = GL_TRIANGLES,
    DT_TRIANGLE_FAN = GL_TRIANGLE_FAN,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP
} DRAW_TYPE;

typedef enum{
    ZERO_VALUE = GL_ZERO,
    REPLEASE_VALUE = GL_REPLACE,
    INCR_CLAMP = GL_INCR,
    DECR_CLAMP = GL_DECR,
    INCR_REPEAT = GL_INCR_WRAP,
    DECR_REPEAT = GL_DECR_WRAP,
    KEEP_VALUE = GL_KEEP,
    INVERT_BIT_VALUE = GL_INVERT
} StencilOp;

typedef enum{
    ADD              = GL_FUNC_ADD,
    SRC_SUBREACT_DST = GL_FUNC_SUBTRACT,
    DST_SUBTRACT_SRC = GL_FUNC_REVERSE_SUBTRACT
} BlendMode;

typedef enum
{
    RGBA8,
    RGB565
} GLPixelFormat;

typedef enum
{
    DEPTH_DEFAULT       = GL_DEPTH_COMPONENT,
    DEPTH_16            = GL_DEPTH_COMPONENT16,
    DEPTH_24            = GL_DEPTH_COMPONENT24,
    DEPTH_32F           = GL_DEPTH_COMPONENT32F,
    DEPTH32F_STENCIL8   = GL_DEPTH32F_STENCIL8,
    DEPTH24_STENCIL8    = GL_DEPTH24_STENCIL8
} DepthFormat;

typedef enum{
    FIT_WIDTH,
    FIT_HEIGHT,
    NO_BORDER,
    SHOW_ALL
} ResolutionPolicy;

#define BUFFER_OFFSET_FLOAT(offset) (GLubyte *)NULL + ((offset) * sizeof(float))
#define BUFFER_OFFSET_BYTE(offset) (GLubyte *)NULL + (offset)

inline void CheckError()
{
	GLenum err = glGetError();
	if(GL_INVALID_ENUM == err || GL_INVALID_INDEX)
	{
		std::cout<<"invalid input"<<std::endl;
		return;
	}
}

#endif