#pragma once
#include "gpscreen.h"
#include "glfunc.h"

#include "GPGLShaderProgram.h"
#include "GPGLShader.h"

#include "GPMatrix4.h"

class GLSkyBoxScreen :
	public GPEngine3D::Screen
{
public:
	GLSkyBoxScreen(void);
	virtual ~GLSkyBoxScreen(void);

	virtual void update(double deltaTime);
	virtual void present();

private:
	GLuint posVBO;
	GLuint uvVBO;
	GLuint idxVBO;
	GLuint VAO;
	GLuint tex;

	static const int ss = 400;

	GLuint posHandle,uvHandle, matHandle;

	GPEngine3D::GLShaderProgram program;

	GPEngine3D::mat4 MVP;
};

