#include "GPScreen.h"
#include "glfunc.h"

#include "GPGLShaderProgram.h"
#include "GPGLShader.h"
#include "GPVector.h"
#include "GPMatrix4.h"

class GLLightScreen : 
	public GPEngine3D::Screen
{
public:
	GLLightScreen();
	~GLLightScreen();

	void createCube(const GPEngine3D::vec3f &pos, float halfSize);

	void update(double deltaTime) override;
	void present() override{};
private:
	GLuint cubeVAO;
	GLuint cubeVBO;
	GLuint cubeIdxVBO;

	GLint posHandle;
	GLint normalHandle;
	GLint lightPosHandle;
	GLint mvpHandle;
	GLint mHandle;
	GLint shininessHandle; 

	GPEngine3D::GLShaderProgram program;

	GPEngine3D::vec3f lightPos;
	GPEngine3D::mat4 P;
	GPEngine3D::mat4 MV;
};