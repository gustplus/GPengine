#include "stdafx.h"
#include "GLWaveScreen.h"
#include "GPKeyHandler.h"

using namespace GPEngine3D;

static GLfloat kernels[][9] = {
								{0, 0, 0,
								 0, 0, 0,
								 0, 0, 0},
							   {0, -1, 0,
								-1,  5, -1,
								0, -1, 0},
							  	{1, 1, 1,
								 1, 1, 1,
								 1, 1, 1},
								{0, 1, 0,
								 1, 5, 1,
								 0, 1, 0},
								{-4, -4, 0,
								 -4, 12, 0,
								  0,  0, 0},
								{ 2, 0, 2,
								  0, 0, 0,
								  3, 0, -6
								 }
								};


const GPEngine3D::vec3f GLWaveScreen::mirriorNormal = vec3f(0, 0, 1);

GLWaveScreen::GLWaveScreen(void)
{
	time = 0;
	glClearColor(0, 0, 1, 1);
	
	P.frustum(-0.01, 0.01, -0.006, 0.006, 0.01, 1000);

	

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
	delete[] buf;


	vertShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/skybox.vert", ShaderType::VERTEX_SHADER);
	fragShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/skybox.frag", ShaderType::FRAG_SHADER);
	skyProgram.attachShader(vertShader);
	skyProgram.attachShader(fragShader);
	skyProgram.link();

	skyMatHandle = glGetUniformLocation(skyProgram, "vMVP");

	GLfloat skyVertices[] = {-size, size + 70, size, -1, 1, 1,
						   -size, -size + 70, size, -1, -1, 1,
						   size, size + 70, size, 1, 1, 1,
						   size, -size + 70, size, 1, -1, 1,

						   -size, size + 70, -size,-1, 1, -1,
						   -size, -size + 70, -size, -1, -1, -1,
						   size, size + 70, -size, 1, 1, -1,
						   size, -size + 70, -size, 1, -1, -1,
							};

	glGenVertexArrays(1, &skyVAO);
	glBindVertexArray(skyVAO);
	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
	posHandle = glGetAttribLocation(skyProgram, "vPos");
	glEnableVertexAttribArray(posHandle);
	glVertexAttribPointer(posHandle, 3, GL_FLOAT, false, 24, 0);
	uvHandle = glGetAttribLocation(skyProgram, "vUV");
	glEnableVertexAttribArray(uvHandle);
	glVertexAttribPointer(uvHandle, 3, GL_FLOAT, false, 24, (GLvoid *)(NULL + 12));

	GLushort skyIndices[] = { 	0, 1, 2, 1, 3, 2,
								4, 5, 0, 0, 5, 1,
								2, 3, 6, 3, 7, 6,
								6, 7, 5, 6, 5, 4,
								0, 6, 4, 0, 2, 6,
								1, 5, 3, 3, 5, 7 };
	glGenBuffers(1, &idxVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyIndices), skyIndices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/mirrior.vert", ShaderType::VERTEX_SHADER);
	fragShader = new GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/mirrior.frag", ShaderType::FRAG_SHADER);
	mirriorProgram.attachShader(vertShader);
	mirriorProgram.attachShader(fragShader);
	mirriorProgram.link();

	mirriorMatHandle = glGetUniformLocation(mirriorProgram, "vMVP");

	GLubyte grayBuf[2][2][3];
	for (int row = 0; row < 2; ++row)
	{
		for (int col = 0; col < 2; ++col)
		{
			grayBuf[row][col][0] = (GLubyte)200;
			grayBuf[row][col][1] = (GLubyte)200;
			grayBuf[row][col][2] = (GLubyte)200;
		}
	}
	glGenTextures(1, &mirriorBackTex);
	glBindTexture(GL_TEXTURE_2D, mirriorBackTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, grayBuf);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenVertexArrays(1, &mirriorVAO);
	glBindVertexArray(mirriorVAO);
	GLfloat mirriorVertices[] = {-mirriorSize, 2 * mirriorSize, -20,
								 -mirriorSize, 0,			    -20,
								  mirriorSize, 0, 				-20,
								  mirriorSize, 2 * mirriorSize, -20};
	GLushort mirriorIndices[] = { 0, 1, 2, 0, 2, 3};
	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mirriorVertices), mirriorVertices, GL_STATIC_DRAW);
	posHandle = glGetAttribLocation(mirriorProgram, "vPos");
	glEnableVertexAttribArray(posHandle);
	glVertexAttribPointer(posHandle, 3, GL_FLOAT, false, 0, 0);
	glGenBuffers(1, &idxVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mirriorIndices), mirriorIndices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLfloat screenVertices[] = {-1, -1,
								1, -1,
								1, 1,
								-1, 1};
	GLubyte screenIndices[] = {0, 1, 2,
							   0, 2, 3};
	GLShaderPtr vsh = GP_NEW GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/convolution.vert", ShaderType::VERTEX_SHADER);
	GLShaderPtr fsh = GP_NEW GLShader("D:/GitHub/GPEngine/GPGame/Shaders/GLSL/convolution.frag", ShaderType::FRAG_SHADER);
	filterProgram.attachShader(vsh);
	filterProgram.attachShader(fsh);
	filterProgram.link();

	glGenBuffers(1, &posVBO);
	glGenBuffers(1, &idxVBO);
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(screenIndices), screenIndices, GL_STATIC_DRAW);
	posHandle = glGetAttribLocation(filterProgram, "vPos");
	powerHandle = glGetUniformLocation(filterProgram, "power");
	kernelHandle = glGetUniformLocation(filterProgram, "kernel");
	glEnableVertexAttribArray(posHandle);
	glVertexAttribPointer(posHandle, 2, GL_FLOAT, false, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	createMirriorFrameBuffer();
	createScreenFrameBuffer();
}


GLWaveScreen::~GLWaveScreen(void)
{
	glDeleteBuffers(1, &idxVBO);
	glDeleteBuffers(1, &posVBO);
	glDeleteVertexArrays(1, &skyVAO);
	glDeleteVertexArrays(1, &mirriorVAO);
	glDeleteVertexArrays(1, &skyVAO);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteTextures(1, &tex);
	glDeleteTextures(1, &mirriorTex);
	glDeleteTextures(1, &mirriorBackTex);
}

void GLWaveScreen::update(double deltaTime)
{
	
	static bool isLine = false;
	static float posX = 0;
	static float posY = 10;
	static float posZ = 30;
	time += deltaTime * 6;

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_LEFT_ARROW))
	{
		posX += deltaTime * 80;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_RIGHT_ARROW))
	{
		posX -= deltaTime * 80;
	}
	if (posX > size)
	{
		posX = size;
	}
	if (posX < -size)
	{
		posX = -size;
	}

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_UP_ARROW))
	{
		posZ -= 80 * deltaTime;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_DOWN_ARROW))
	{
		posZ += 80 * deltaTime;
	}
	if (posZ > size)
	{
		posZ = size;
	}
	if (posZ < -size)
	{
		posZ = -size;
	}

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_PAGE_DOWN))
	{
		posY -= 80 * deltaTime;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_PAGE_UP))
	{
		posY += 80 * deltaTime;
	}
	if (posY > 2 * size - 50)
	{
		posY = 2 * size - 50;
	}
	if (posY < 0)
	{
		posY = 0;
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

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	vec3f lookAtP(0, 30, -20);
	vec3f pos(posX, posY, posZ);
	vec3f reflectV = reflect(mirriorNormal, lookAtP - pos);
	vec3f refPos = lookAtP + reflectV;
	MVP.indetify();
	mat4 transMat;
	transMat.setScale(-1, 1, 1);
	MVP = MVP * transMat;
	transMat.lookAt(refPos, lookAtP, vec3f(0, 1, 0));
	MVP = MVP * transMat;
	MVP = P * MVP;
	glCullFace(GL_FRONT);
	renderScene();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	drawScreenRect();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK_LEFT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	MVP.indetify();
	transMat.lookAt(pos, lookAtP, vec3f(0, 1, 0));
	MVP = MVP * transMat;
	MVP = P * MVP;
	glCullFace(GL_BACK);
	renderScene();

	glDisable(GL_CULL_FACE);
	mirriorProgram.use();
	if(posZ > -20)
	{
		glBindTexture(GL_TEXTURE_2D, screenTex);	
	}else{
		glBindTexture(GL_TEXTURE_2D, mirriorBackTex);
	}
	glUniformMatrix4fv(mirriorMatHandle, 1, false, MVP.m);
	glBindVertexArray(mirriorVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

void GLWaveScreen::renderScene()
{
	glFrontFace(GL_CW);
	waveProgram.use();
	glUniform1f(timeHandle, time);
	glUniformMatrix4fv(pMatHandle, 1, false, MVP.m);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT, NULL);

	glFrontFace(GL_CCW);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	skyProgram.use();
	glUniformMatrix4fv(skyMatHandle, 1, false, MVP.m);
	glBindVertexArray(skyVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
}

void GLWaveScreen::present()
{
	
}

void GLWaveScreen::createMirriorFrameBuffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 800, 480);
	glGenTextures(1, &mirriorTex);
	glBindTexture(GL_TEXTURE_2D, mirriorTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO); 
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirriorTex, 0);

	GLuint FBOStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if(FBOStatus == GL_FRAMEBUFFER_COMPLETE)
	{
		printf("fbo ok!\n");
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLWaveScreen::createScreenFrameBuffer()
{
	glGenFramebuffers(1, &scrFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scrFBO);
	glGenRenderbuffers(1, &scrDepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, scrDepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 800, 480);
	glGenTextures(1, &screenTex);
	glBindTexture(GL_TEXTURE_2D, screenTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, scrDepthRBO); 
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTex, 0);

	GLuint FBOStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if(FBOStatus == GL_FRAMEBUFFER_COMPLETE)
	{
		printf("screen fbo ok!\n");
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLWaveScreen::drawScreenRect()
{	
	static int kernelIdx = 0;
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
	if(KeyHandler::getInstance()->isKeyDown('Y'))
	{
		kernelIdx = 5;
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scrFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	filterProgram.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mirriorTex);
	glUniform1fv(kernelHandle, 9, kernels[kernelIdx]);
	glUniform1f(powerHandle, 1.0f); 
	glBindVertexArray(screenVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
}