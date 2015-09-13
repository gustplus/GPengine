#include "GLLightScreen.h"
#include "GPKeyHandler.h"

US_NS_GP;

GLLightScreen::GLLightScreen():lightPos(0, 0, 0)
{
	glClearColor(0, 0, 0, 1);
	P.frustum(-0.01, 0.01, -0.006, 0.006, 0.01, 1000);

	GLShaderPtr vertShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/pointlight.vert", ShaderType::VERTEX_SHADER);
	GLShaderPtr fragShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/pointlight.frag", ShaderType::FRAG_SHADER);

	program.attachShader(vertShader);
	program.attachShader(fragShader);
	program.link();
	posHandle = glGetAttribLocation(program, "vPos");
	normalHandle = glGetAttribLocation(program, "vNormal");
	lightPosHandle = glGetUniformLocation(program, "vLightPos");
	mvpHandle = glGetUniformLocation(program, "vMVP");
	mHandle = glGetUniformLocation(program, "vMV");
	shininessHandle = glGetUniformLocation(program, "shininess");

	createCube(vec3f(0, 0, -25), 10);
}

GLLightScreen::~GLLightScreen()
{

}

void GLLightScreen::createCube(const vec3f &pos, float halfSize)
{
	GLfloat cube[] = {  pos.x - halfSize, pos.y - halfSize, pos.z + halfSize, 0, 0, 1,
						pos.x + halfSize, pos.y - halfSize, pos.z + halfSize, 0, 0, 1,
						pos.x + halfSize, pos.y + halfSize, pos.z + halfSize, 0, 0, 1,

						pos.x - halfSize, pos.y - halfSize, pos.z + halfSize, 0, 0, 1,
						pos.x + halfSize, pos.y + halfSize, pos.z + halfSize, 0, 0, 1,
						pos.x - halfSize, pos.y + halfSize, pos.z + halfSize, 0, 0, 1};
						// ,

						// pos.x - halfSize, pos.y + halfSize, pos.z - halfSize,-1, 1, 
						// pos.x - halfSize, pos.y - halfSize, pos.z - halfSize, -1, -1, 
						// pos.x + halfSize, pos.y + halfSize, pos.z - halfSize, 1, 1, 
						// pos.x + halfSize, pos.y - halfSize, pos.z - halfSize, 1, -1};
	// GLushort indices[] = { 0, 1, 2, 1, 3, 2,
	// 					   4, 5, 0, 0, 5, 1,
	// 					   2, 3, 6, 3, 7, 6,
	// 					   6, 7, 5, 6, 5, 4,
	// 					   0, 6, 4, 0, 2, 6,
	// 					   1, 5, 3, 3, 5, 7 };
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glEnableVertexAttribArray(posHandle);
	glVertexAttribPointer(posHandle, 3, GL_FLOAT, false, 24, NULL);
	glEnableVertexAttribArray(normalHandle);
	glVertexAttribPointer(normalHandle, 3, GL_FLOAT, false, 24, (GLvoid *)(NULL + 12));
	// glGenBuffers(1, &cubeIdxVBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIdxVBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLLightScreen::update(double deltaTime)
{	
	static float x = 0;
	static float y = 0;
	static float z = 0;
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_LEFT_ARROW))
	{
		x -= deltaTime * 30;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_RIGHT_ARROW))
	{
		x += deltaTime * 30;
	}
	if(KeyHandler::getInstance()->isKeyDown('W'))
	{
		z -= deltaTime * 30;
	}
	if(KeyHandler::getInstance()->isKeyDown('S'))
	{
		z += deltaTime * 30;
	}

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_UP_ARROW))
	{
		y += deltaTime * 30;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_DOWN_ARROW))
	{
		y -= deltaTime * 30;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MV.indetify();
	program.use();
	mat4 mvp = P * MV;
	glUniformMatrix4fv(mvpHandle, 1, false, mvp.m);
	glUniformMatrix4fv(mHandle, 1, false, MV.m);
	glUniform3f(lightPosHandle, x, y, z);
	glUniform1f(shininessHandle, 0.01f);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, NULL, 6);
}