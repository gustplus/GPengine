#pragma once
#include "gpscreen.h"
#include "glfunc.h"

#include "GPGLShaderProgram.h"
#include "GPGLShader.h"

#include "GPMatrix4.h"

class GLTestScreen :
	public GPEngine3D::Screen
{
public:
	GLTestScreen(void);
	virtual ~GLTestScreen(void);

	virtual void update(double deltaTime);
	virtual void present();

private:
	GLuint posVBO;
	GLuint idxVBO;
	GLuint VAO;
	float time;

	static const int cellSize = 4;
	static const int width = 51;
	static const int height = 51;

	GLuint posHandle, timeHandle, matHandle;

	GPEngine3D::GLShaderProgram program;

	GPEngine3D::mat4 MVP;
};

