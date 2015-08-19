#include "GLFilterScreen.h"
#include "types.h"
#include "GPKeyhandler.h"

using namespace GPEngine3D;

GLFilterScreen::GLFilterScreen(void)
{
	GLfloat vertices[] = {-1, -1,
						  1, -1,
						  1, 1,
						  -1, 1};
	GLubyte indices[] = {0, 1, 2,
						 0, 2, 3};

	TCHAR szPath[MAX_PATH];
		if( GetModuleFileName( NULL, szPath, MAX_PATH ) )
		{
			 printf("GetModuleFileName %s\n", szPath);
		}

	GLShaderPtr vsh = GP_NEW GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/twirlFilter.vert", ShaderType::VERTEX_SHADER);
	GLShaderPtr fsh = GP_NEW GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/twirlFilter.frag", ShaderType::FRAG_SHADER);

	program.attachShader(vsh);
	program.attachShader(fsh);

	program.link();

	glGenBuffers(1, &posVBO);
	glGenBuffers(1, &idxVBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	posHandle = glGetAttribLocation(program, "vPos");
	powerHandle = glGetUniformLocation(program, "power");
	glEnableVertexAttribArray(posHandle);
	glVertexAttribPointer(posHandle, 2, GL_FLOAT, false, 0, 0);
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
	glBindTexture(GL_TEXTURE_2D, tex);
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);  
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s, s, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
	free(buf);
}


GLFilterScreen::~GLFilterScreen(void)
{
}

void GLFilterScreen::update(double deltaTime)
{
	static float power = 0;
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_LEFT_ARROW))
	{
		power += deltaTime * 20;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_RIGHT_ARROW))
	{
		power -= deltaTime * 20;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.use();
	glUniform1f(posHandle, power);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}

void GLFilterScreen::present()
{
	
}