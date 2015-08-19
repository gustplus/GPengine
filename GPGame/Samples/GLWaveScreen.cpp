#include "stdafx.h"
#include "GLWaveScreen.h"
#include "GPKeyHandler.h"

using namespace GPEngine3D;

GLTestScreen::GLTestScreen(void)
{
	time = 0;

	 GLfloat vertices[width][height][3];
	 for (int row = 0; row < height; ++row)
	 {
	 	for (int col = 0; col < width; ++col)
	 	{
	 		vertices[row][col][0] = ((int)(height * 0.5) - row) * cellSize;
	 		vertices[row][col][1] = 0; 
	 		vertices[row][col][2] = (col - (int)(width * 0.5)) * cellSize;
	 	}
	 }

	 GLuint indices[(width - 1) * (height - 1) * 6];
	 GLuint *id = indices;
	 for (int row = 0; row < height - 1; ++row)
	 {
	 	GLuint offset = row * width;
	 	for (int col = 0; col < width - 1; ++col)
	 	{
	 		GLuint idx0 = offset + col;
	 		GLuint idx1 = idx0 + width;
	 		GLuint idx2 = idx1 + 1;
	 		GLuint idx3 = idx0 + 1;
	 		if((row + col) % 2 == 0)
	 		{
	 			*id++ = idx0;
	 			*id++ = idx1;
	 			*id++ = idx2;
	 			*id++ = idx0;
	 			*id++ = idx2;
	 			*id++ = idx3;
	 		}else{
	 			*id++ = idx0;
	 			*id++ = idx1;
	 			*id++ = idx3;
	 			*id++ = idx1;
	 			*id++ = idx2;
	 			*id++ = idx3;
	 		}
	 	}
	 }

	 GLShaderPtr vertShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/wave.vert", ShaderType::VERTEX_SHADER);
	 GLShaderPtr fragShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/wave.frag", ShaderType::FRAG_SHADER);

	 program.attachShader(vertShader);
	 program.attachShader(fragShader);
	 program.link();

	 glGenBuffers(1, &posVBO);
	 glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	 glGenVertexArrays(1, &VAO);
	 glBindVertexArray(VAO);
	 posHandle = glGetAttribLocation(program, "vPos");
	 timeHandle = glGetUniformLocation(program, "time");
	 matHandle = glGetUniformLocation(program, "vMVP");
	 glEnableVertexAttribArray(posHandle);
	 glVertexAttribPointer(posHandle, 3, GL_FLOAT, false, 0, 0);
	 glGenBuffers(1, &idxVBO);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	 glBindVertexArray(0);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	 glClearColor(1, 1, 0, 1);
	 glEnable(GL_DEPTH_TEST);
}


GLTestScreen::~GLTestScreen(void)
{
}

void GLTestScreen::update(double deltaTime)
{
	static float angle = 0;
	static float posZ = -100;
	static bool isLine = false;

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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	time += deltaTime * 2;

	program.use();
	glUniform1f(timeHandle, time);
	glUniformMatrix4fv(matHandle, 1, false, MVP.m);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT, 0);

}

void GLTestScreen::present()
{
	
}
