#include "stdafx.h"
#include "GLTestScreen.h"
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

	GLShaderPtr vertShader = new GLShader("Shaders/GLSL/wave.vert", ShaderType::VERTEX_SHADER);
	GLShaderPtr fragShader = new GLShader("Shaders/GLSL/wave.frag", ShaderType::FRAG_SHADER);

	waveProgram.attachShader(vertShader);
	waveProgram.attachShader(fragShader);
	waveProgram.link();

	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	posHandle = glGetAttribLocation(waveProgram, "vPos");
	timeHandle = glGetUniformLocation(waveProgram, "time");
	pMatHandle = glGetUniformLocation(waveProgram, "vMVP");
	glEnableVertexAttribArray(posHandle);
	glVertexAttribPointer(posHandle, 3, GL_FLOAT, false, 0, 0);
	glGenBuffers(1, &idxVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	const int s = 128;
	byte *buf = (byte *)malloc(s * s * 3);
	for(int row = 0; row < s; ++row)
	{
		byte *tmp = buf + (row * s * 3);
		for(int col = 0; col < s; ++col)
		{
			byte c = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
			*(tmp++) = (GLubyte)c;
			*(tmp++) = (GLubyte)255;
			*(tmp++) = (GLubyte)255;
		}
	}
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, s, s, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, s, s, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, s, s, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, s, s, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, s, s, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, s, s, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);


	vertShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/skybox.vert", ShaderType::VERTEX_SHADER);
	fragShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/skybox.frag", ShaderType::FRAG_SHADER);

	skyProgram.attachShader(vertShader);
	skyProgram.attachShader(fragShader);
	skyProgram.link();

	matHandle = glGetUniformLocation(skyProgram, "vMVP");

	GLfloat vertices[] = {-size, size, size, -1, 1, 1,
					   -size, -size, size, -1, -1, 1,
					   size, size, size, 1, 1, 1,
					   size, -size, size, 1, -1, 1,

					   -size, size, -size,-1, 1, -1,
					   -size, -size, -size, -1, -1, -1,
					   size, size, -size, 1, 1, -1,
					   size, -size, -size, 1, -1, -1,
						};

	glGenVertexArrays(1, &SkyVAO);
	glBindVertexArray(SkyVAO);
	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	posHandle = glGetAttribLocation(skyProgram, "vPos");
	glEnableVertexAttribArray(posHandle);
	glVertexAttribPointer(posHandle, 3, GL_FLOAT, false, 24, 0);
	uvHandle = glGetAttribLocation(skyProgram, "vUV");
	glEnableVertexAttribArray(uvHandle);
	glVertexAttribPointer(uvHandle, 3, GL_FLOAT, false, 24, (GLvoid *)(NULL + 12));

	GLushort indices[] = { 0, 1, 2, 1, 3, 2,
					   4, 5, 0, 0, 5, 1,
					   2, 3, 6, 3, 7, 6,
					   6, 7, 5, 6, 5, 4,
					   0, 6, 4, 0, 2, 6,
					   1, 5, 3, 3, 5, 7 };
	glGenBuffers(1, &idxVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


GLTestScreen::~GLTestScreen(void)
{
}

void GLTestScreen::update(double deltaTime)
{
	static float angle = 0;
	static bool isLine = false;
	static float posZ = 0;

	glClearColor(1, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_LEFT_ARROW))
	{
		angle -= deltaTime * 80;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_RIGHT_ARROW))
	{
		angle += deltaTime * 80;
	}
	if (angle > 360.0f)
	{
		angle -= 360.0f;
	}
	if (angle < 0.0f)
	{
		angle += 360.0f;
	}

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_UP_ARROW))
	{
		posZ += 80 * deltaTime;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_DOWN_ARROW))
	{
		posZ -= 80 * deltaTime;
	}
	if (posZ > ss)
	{
		posZ = ss;
	}
	if (posZ < -ss)
	{
		posZ = -ss;
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

	MVP.frustum(-0.01, 0.01, -0.006, 0.006, 0.01, 1000);
	mat4 transMat;
	transMat.setTranslation(vec3f(0, -15, posZ));
	MVP = MVP * transMat;
	transMat.setRotation(angle * TO_RADIANS, vec3f(0, 1, 0));
	MVP = MVP * transMat;

	time += deltaTime * 2;

	waveProgram.use();
	glUniform1f(timeHandle, time);
	glUniformMatrix4fv(pMatHandle, 1, false, MVP.m);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	skyProgram.use();
	glUniformMatrix4fv(skyMatHandle, 1, false, MVP.m);

	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

void GLTestScreen::present()
{
	
}
