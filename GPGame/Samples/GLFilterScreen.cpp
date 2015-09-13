#include "GLFilterScreen.h"
#include "types.h"
#include "GPKeyhandler.h"
#include "GPMath.h"

using namespace GPEngine3D;

static GLfloat kernels[][9] = {
								{0, 0, 0,
								 0, 0, 0,
								 0, 0, 0},
							   {-1, -1, -1,
								-1,  8, -1,
								-1, -1, -1},
							  	{1, 1, 1,
								 1, 1, 1,
								 1, 1, 1},
								{0, 1, 0,
								 1, 5, 1,
								 0, 1, 0},
								{-4, -4, 0,
								 -4, 12, 0,
								  0,  0, 0}
								};

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

	GLShaderPtr vsh = GP_NEW GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/convolution.vert", ShaderType::VERTEX_SHADER);
	GLShaderPtr fsh = GP_NEW GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/convolution.frag", ShaderType::FRAG_SHADER);

	program.attachShader(vsh);
	program.attachShader(fsh);

	program.link();
	program.printAttributes();
	program.printUniforms();

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
	kernelHandle = glGetUniformLocation(program, "kernel");
	glEnableVertexAttribArray(posHandle);
	glVertexAttribPointer(posHandle, 2, GL_FLOAT, false, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	const int s = 64;
	byte *buf = (byte *)malloc(s * s * 3);
	for(int row = 0; row < s; ++row)
	{
		byte *tmp = buf + (row * s * 3);
		for(int col = 0; col < s; ++col)
		{
			// byte c = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
			*(tmp++) = Math::random();
			*(tmp++) = Math::random();
			*(tmp++) = Math::random();
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
	static float power = 1.0f;
	static int kernelIdx = 0;
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_LEFT_ARROW))
	{
		power += 0.1;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_RIGHT_ARROW))
	{
		power -= 0.1;
	}
	if(KeyHandler::getInstance()->isKeyDown('Q'))
	{
		kernelIdx = 0;
	}
	if(KeyHandler::getInstance()->isKeyDown('W'))
	{
		kernelIdx = 1;
	}
	if(KeyHandler::getInstance()->isKeyDown('E'))
	{
		kernelIdx = 2;
	}
	if(KeyHandler::getInstance()->isKeyDown('R'))
	{
		kernelIdx = 3;
	}
	if(KeyHandler::getInstance()->isKeyDown('T'))
	{
		kernelIdx = 4;
	}
	kernelIdx %= sizeof(kernels)/sizeof(kernels[0]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.use();
	glUniform1f(powerHandle, power);
	glUniform1fv(kernelHandle, 9, kernels[kernelIdx]);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}

void GLFilterScreen::present()
{
	
}