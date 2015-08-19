#include "stdafx.h"
#include "GLGemotryShaderScene.h"
#include "GPKeyhandler.h"

using namespace GPEngine3D;

GLGemotryShaderScene::GLGemotryShaderScene(void)
{
	GLfloat vertices[] = {-10, 0, -10, 
						  -10, 0, 10,
						  10, 0, 10,
						  10, 0, -10};
	
	GLubyte indices[] = {0, 1, 3, 2, 3, 1};

	GLShaderPtr vsh = GP_NEW GLShader("Shaders/GLSL/div.vert", ShaderType::VERTEX_SHADER);
	GLShaderPtr gsh = GP_NEW GLShader("Shaders/GLSL/div.geom", ShaderType::GEOMETRY_SHADER);
	GLShaderPtr fsh = GP_NEW GLShader("Shaders/GLSL/div.frag", ShaderType::FRAG_SHADER);

	program.attachShader(vsh);
	program.attachShader(gsh);
	program.attachShader(fsh);
	program.link();

	glProgramParameteri(program, GL_GEOMETRY_INPUT_TYPE, GL_TRIANGLES_ADJACENCY);
	glProgramParameteri(program, GL_GEOMETRY_OUTPUT_TYPE, GL_TRIANGLE_STRIP_ADJACENCY);
	glProgramParameteri(program, GL_GEOMETRY_VERTICES_OUT, 256);

	posHandle = glGetAttribLocation(program, "vPos");
	divHandle = glGetUniformLocation(program, "gDiv");
	gMatHandle = glGetUniformLocation(program, "gMVP");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(posHandle);
	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(posHandle, 3, GL_FLOAT, false, 0, 0);
	glGenBuffers(1, &idxVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


GLGemotryShaderScene::~GLGemotryShaderScene(void)
{
	
}

void GLGemotryShaderScene::update(double deltaTime)
{
	static float angle = 0;
	static float posZ = -60;
	static bool isLine = true;

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_LEFT_ARROW))
	{
		angle -= deltaTime * 40;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_RIGHT_ARROW))
	{
		angle += deltaTime * 40;
	}
	if (angle > 360.0f)
	{
		angle -= 360.0f;
	}
	if (angle < 0.0f)
	{
		angle += 360.0f;
	}

	if(KeyHandler::getInstance()->isKeyDown('L'))
	{
		isLine = !isLine;
	}
	if(isLine)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_UP_ARROW))
	{
		posZ += 32 * deltaTime;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_DOWN_ARROW))
	{
		posZ -= 32 * deltaTime;
	}

	MVP.frustum(-0.01, 0.01, -0.006, 0.006, 0.01, 1000);
	 mat4 transMat;
	 transMat.setTranslation(vec3f(0, -15, posZ));
	 MVP = MVP * transMat;
	 transMat.setRotation(angle * TO_RADIANS, vec3f(0, 1, 0));
	 MVP = MVP * transMat;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);

	program.use();
	glUniform1i(divHandle, 2);
	glUniformMatrix4fv(gMatHandle, 1, false, MVP.m);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
	
}

void GLGemotryShaderScene::present()
{

}
