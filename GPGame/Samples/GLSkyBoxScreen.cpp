#include "stdafx.h"
#include "GLSkyBoxScreen.h"
#include "GPKeyHandler.h"

using namespace GPEngine3D;

GLSkyBoxScreen::GLSkyBoxScreen(void)
{
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


	 GLShaderPtr vertShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/skybox.vert", ShaderType::VERTEX_SHADER);
	 GLShaderPtr fragShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/skybox.frag", ShaderType::FRAG_SHADER);

	 program.attachShader(vertShader);
	 program.attachShader(fragShader);
	 program.link();

	 matHandle = glGetUniformLocation(program, "vMVP");

	 GLfloat vertices[] = {-ss, ss, ss, -1, 1, 1,
						   -ss, -ss, ss, -1, -1, 1,
						   ss, ss, ss, 1, 1, 1,
						   ss, -ss, ss, 1, -1, 1,

						   -ss, ss, -ss,-1, 1, -1,
						   -ss, -ss, -ss, -1, -1, -1,
						   ss, ss, -ss, 1, 1, -1,
						   ss, -ss, -ss, 1, -1, -1,
							};

	 glGenVertexArrays(1, &VAO);
	 glBindVertexArray(VAO);
	 glGenBuffers(1, &posVBO);
	 glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	 posHandle = glGetAttribLocation(program, "vPos");
	 glEnableVertexAttribArray(posHandle);
	 glVertexAttribPointer(posHandle, 3, GL_FLOAT, false, 24, 0);
	 uvHandle = glGetAttribLocation(program, "vUV");
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

	 glClearColor(1, 1, 0, 1);
	 glEnable(GL_DEPTH_TEST);
}


GLSkyBoxScreen::~GLSkyBoxScreen(void)
{
}

void GLSkyBoxScreen::update(double deltaTime)
{
	glDisable(GL_CULL_FACE);
	static float angle = 0;
	static float posZ = 0;
	static bool isLine = false;

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

	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	MVP.frustum(-0.01, 0.01, -0.006, 0.006, 0.01, 1000);
	 mat4 transMat;
	 transMat.setTranslation(vec3f(0, 0, posZ));
	 MVP = MVP * transMat;
	 transMat.setRotation(angle * TO_RADIANS, vec3f(0, 1, 0));
	 MVP = MVP * transMat;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.use();
	glUniformMatrix4fv(matHandle, 1, false, MVP.m);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

}

void GLSkyBoxScreen::present()
{
	
}
