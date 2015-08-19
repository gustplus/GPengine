#pragma once

#include "gpscreen.h"
#include "glfunc.h"
#include "GPGLShaderProgram.h"
#include "GPMatrix4.h"

namespace GPEngine3D{

class GLGemotryShaderScene :
	public GPEngine3D::Screen
{
public:
	GLGemotryShaderScene(void);
	virtual ~GLGemotryShaderScene(void);

	void update(double deltaTime) override;
	void present() override;

private:
	GLuint posVBO;
	GLuint idxVBO;
	GLuint VAO;

	static const int cellSize = 4;
	static const int width = 51;
	static const int height = 51;

	GLuint posHandle, divHandle, gMatHandle;

	GPEngine3D::GLShaderProgram program;

	GPEngine3D::mat4 MVP;
};

}
