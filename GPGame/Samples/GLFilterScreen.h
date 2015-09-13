#pragma once

#include "GPScreen.h"
#include "glfunc.h"

#include "GPGLShaderProgram.h"
#include "GPGLShader.h"

class GLFilterScreen :
	public GPEngine3D::Screen
{
public:
	GLFilterScreen(void);
	virtual ~GLFilterScreen(void);

	void update(double deltaTime) override;
	void present() override;

private:
	GLuint posVBO;
	GLuint idxVBO;
	GLuint VAO;
	float time;

	GLuint tex;

	GLuint posHandle, powerHandle, kernelHandle;

	GPEngine3D::GLShaderProgram program;
};

