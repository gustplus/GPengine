#include "stdafx.h"
#include "SoftwareRendererScreen.h"
#include "GPKeyHandler.h"
#include "glfunc.h"

#include "GPVector.h"
#include "types.h"

using namespace GPEngine3D; 

SoftwareRendererScreen::SoftwareRendererScreen(void):
	view(PixelFormat::RGB, 8, 24, true, 800, 480),
	axis(1.0f, 1.0f, 1.0f)
{
	view.setClearColor(ColorRGBA(1,1,0,1));

	view.enable(ATTR_FLAG::DEPTH_BUFFER);

	projMat.frustum(-0.01, 0.01, -0.006, 0.006, 0.01, 1000);

	static const int s = 32;
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
	tex.initWithBytes((byte *)buf, s, s);
	tex.generateMipmap();

	GLfloat uvs[] = {0.0f, 0.0f,
					 0.0f, 1.0f,
					 1.0f, 0.0f,
					 1.0f, 1.0f,
					 0.0f, 0.0f,
					 0.0f, 1.0f,
					 1.0f, 0.0f,
					 1.0f, 1.0f};
	//cube0.vertexAttribTexCoordPointer(false, 0, uvs, 8);
	//cube0.setTexture(&tex);
	cube1.vertexAttribTexCoordPointer(false, 0, uvs, 8);
	//cube1.setTexture(&tex);
	cube2.vertexAttribTexCoordPointer(false, 0, uvs, 8);
	cube2.setTexture(&tex);
	cube3.vertexAttribTexCoordPointer(false, 0, uvs, 8);
	cube3.setTexture(&tex);
	
	GLubyte colors[] = {255, 0, 0, 255,
						0, 255, 0, 255,
						0, 0, 255, 255,
						255, 255, 255, 255,
						0, 0, 255, 255,
						0, 255, 0, 255,
						255, 0, 0, 255,
						255, 255, 255, 255};

	cube0.vertexAttribColorPointer(false, 0, colors, 8);
	cube1.vertexAttribColorPointer(false, 0, colors, 8);
	//cube2.vertexAttribColorPointer(false, 0, colors, 8);
	cube3.vertexAttribColorPointer(false, 0, colors, 8);

	view.cullFace(CCW);

	GLushort indices[] = { 0, 1, 2, 1, 3, 2,
						   4, 5, 0, 0, 5, 1,
						   2, 3, 6, 3, 7, 6,
						   6, 7, 5, 6, 5, 4,
						   0, 6, 4, 0, 2, 6,
						   1, 5, 3, 3, 5, 7 };

	cube0.vertexAttribIndexPointer(0, indices, 36);
	cube1.vertexAttribIndexPointer(0, indices, 36);
	cube2.vertexAttribIndexPointer(0, indices, 36);
	cube3.vertexAttribIndexPointer(0, indices, 36);

	angle = 0;
	// update(0);
}


SoftwareRendererScreen::~SoftwareRendererScreen(void)
{
}

void SoftwareRendererScreen::update(double deltaTime){
	static float offsetY = 20;
	static float offsetX = 20;
	static float offsetZ = -60;

	static int faceIdx = 0;
	static int faceCount = 1;

	static int scale = 1;
	
	if(KeyHandler::getInstance()->isKeyDown('R'))
	{
		angle -= deltaTime * 40;
	}

	if(KeyHandler::getInstance()->isKeyDown('Q'))
	{
		angle += deltaTime * 40;
	}

	if (angle > 360.0f)
	{
		angle -= 360.0f;
	}

	if (angle > 360.0f)
	{
		angle -= 360.0f;
	}

	scaleMat.indetify();
	scaleMat.setScale(vec3f(0.5f, 0.5f, 0.5f));

	rotateMat.indetify();
	rotateMat.setRotation(TO_RADIANS * angle, axis);

	int x = KeyHandler::getInstance()->mouseX;
	int y = KeyHandler::getInstance()->mouseY;

	offsetZ += KeyHandler::getInstance()->rollDir * deltaTime * 28;

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_SHIFT))
	{
		scale = 3;
	}

	if(KeyHandler::getInstance()->isKeyDown('W'))
	{
		offsetZ += 24 * deltaTime * scale;
	}
	if(KeyHandler::getInstance()->isKeyDown('S'))
	{
		offsetZ -= 24 * deltaTime * scale;
	}
	scale = 1;
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_LEFT_ARROW))
	{
		offsetX -= 24 * deltaTime;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_RIGHT_ARROW))
	{
		offsetX += 24 * deltaTime;
	}

	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_UP_ARROW))
	{
		offsetY += 32 * deltaTime;
	}
	if(KeyHandler::getInstance()->isKeyDown(KeyHandler::KEY_DOWN_ARROW))
	{
		offsetY -= 32 * deltaTime;
	}

	view.clearBuffer();
	
	float halfX = 10;
	float halfY = 10;
	float halfZ = 10;

	rotateMat = projMat * rotateMat;

	float tmp_z = offsetZ;
	for (int i = 0; i < 50; ++i)
	{
		list0.reset();
		list1.reset();
		list2.reset();
		list3.reset();
		GLfloat vertices[] = { -halfX + offsetX, halfY - offsetY, halfZ + tmp_z,
						   	   -halfX + offsetX, -halfY - offsetY, halfZ + tmp_z,
						   		halfX + offsetX, halfY - offsetY, halfZ + tmp_z,
						   		halfX + offsetX, -halfY - offsetY, halfZ + tmp_z,
								-halfX + offsetX, halfY - offsetY, -halfZ + tmp_z,
								-halfX + offsetX, -halfY - offsetY, -halfZ + tmp_z,
								halfX + offsetX, halfY - offsetY, -halfZ + tmp_z,
								halfX + offsetX, -halfY - offsetY, -halfZ + tmp_z};
		cube0.vertexAttribPosPointer(false, 0, vertices, 8);

		GLfloat vertices1[] = { -halfX - offsetX, halfY + offsetY, halfZ + tmp_z,
							   	-halfX - offsetX, -halfY + offsetY, halfZ + tmp_z,
							   	halfX - offsetX, halfY + offsetY, halfZ + tmp_z,
							   	halfX - offsetX, -halfY + offsetY, halfZ + tmp_z,
								-halfX - offsetX, halfY + offsetY, -halfZ + tmp_z,
								-halfX - offsetX, -halfY + offsetY, -halfZ + tmp_z,
								halfX - offsetX, halfY + offsetY, -halfZ + tmp_z,
								halfX - offsetX, -halfY + offsetY, -halfZ + tmp_z};
		cube1.vertexAttribPosPointer(false, 0, vertices1, 8);

		GLfloat vertices2[] = { -halfX - offsetX, halfY - offsetY, halfZ + tmp_z,
							   	-halfX - offsetX, -halfY - offsetY, halfZ + tmp_z,
							   	halfX - offsetX, halfY - offsetY, halfZ + tmp_z,
							   	halfX - offsetX, -halfY - offsetY, halfZ + tmp_z,
								-halfX - offsetX, halfY - offsetY, -halfZ + tmp_z,
								-halfX - offsetX, -halfY - offsetY, -halfZ + tmp_z,
								halfX - offsetX, halfY - offsetY, -halfZ + tmp_z,
								halfX - offsetX, -halfY - offsetY, -halfZ + tmp_z};
		cube2.vertexAttribPosPointer(false, 0, vertices2, 8);

		GLfloat vertices3[] = { -halfX + offsetX, halfY + offsetY, halfZ + tmp_z,
							   	-halfX + offsetX, -halfY + offsetY, halfZ + tmp_z,
							   	halfX + offsetX, halfY + offsetY, halfZ + tmp_z,
							   	halfX + offsetX, -halfY + offsetY, halfZ + tmp_z,
								-halfX + offsetX, halfY + offsetY, -halfZ + tmp_z,
								-halfX + offsetX, -halfY + offsetY, -halfZ + tmp_z,
								halfX + offsetX, halfY + offsetY, -halfZ + tmp_z,
								halfX + offsetX, -halfY + offsetY, -halfZ + tmp_z};
		cube3.vertexAttribPosPointer(false, 0, vertices3, 8);

		view.removeBackFaces(cube0, 0, 12);
		view.removeBackFaces(cube1, 0, 12);
		view.removeBackFaces(cube2, 0, 12);
		view.removeBackFaces(cube3, 0, 12);
		
		transformPolyObject(cube0, rotateMat);
		transformPolyObject(cube1, rotateMat);
		transformPolyObject(cube2, rotateMat);
		transformPolyObject(cube3, rotateMat);

		// transformPolyObject(cube0, projMat);
		// transformPolyObject(cube1, projMat);
		// transformPolyObject(cube2, projMat);
		// transformPolyObject(cube3, projMat);

		view.insertObjectToRenderList(list0, cube0, 0, 12);
		view.insertObjectToRenderList(list1, cube1, 0, 12);
		view.insertObjectToRenderList(list2, cube2, 0, 12);
		view.insertObjectToRenderList(list3, cube3, 0, 12);
		
		view.CVVClip(list0);
		view.CVVClip(list1);
		view.CVVClip(list2);
		view.CVVClip(list3);

		view.projectionToScreenTransform(list0);
		view.projectionToScreenTransform(list1);
		view.projectionToScreenTransform(list2);
		view.projectionToScreenTransform(list3);

		view.drawArray(list0);
		view.drawArray(list1);
		view.drawArray(list2);
		view.drawArray(list3);
		
		tmp_z -= 30;
	}

	view.swapBuffer();
	// offsetY = -48 + y * 0.2;
	// offsetX = -80 + x * 0.2;
}
void SoftwareRendererScreen::present(){
	
}
