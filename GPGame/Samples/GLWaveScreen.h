#pragma once
#include "gpscreen.h"
#include "glfunc.h"

#include "GPGLShaderProgram.h"
#include "GPGLShader.h"

#include "GPMatrix4.h"

class GLWaveScreen :
	public GPEngine3D::Screen
{
public:
	GLWaveScreen(void);
	virtual ~GLWaveScreen(void);

	virtual void update(double deltaTime);
	virtual void present();

private:
	GLuint posVBO;
	GLuint idxVBO;
	GLuint VAO;
	float time;

	static const int size = 100;

	static const int cellSize = 4;
	static const int width = 51;
	static const int height = 51;

	GLint posHandle, timeHandle, uvHandle, pMatHandle, skyMatHandle, mirriorMatHandle;

	GPEngine3D::GLShaderProgram waveProgram, skyProgram, mirriorProgram, filterProgram;

	GPEngine3D::mat4 P;
	GPEngine3D::mat4 MVP;

	GLuint skyVAO;
	GLuint tex;
	static const int mirriorSize = 30;
	GLuint mirriorVAO;
	GLuint mirriorTex;
	GLuint mirriorBackTex;	

	GLuint FBO;
	GLuint depthRBO;

	GLuint scrFBO;
	GLuint scrDepthRBO;

	GLuint screenTex;
	GLuint kernelHandle, powerHandle;
	GLuint screenVAO;

	static const GPEngine3D::vec3f mirriorNormal;

	void createMirriorFrameBuffer();
	void createScreenFrameBuffer();

	void renderScene();

	void drawScreenRect();
};

