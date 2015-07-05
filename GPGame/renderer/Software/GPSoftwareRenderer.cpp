#include "GPSoftwareRenderer.h"
#include "gp.h"
#include "gpmath.h"

namespace GPEngine3D{
	SoftwareRenderer::SoftwareRenderer(PixelFormat pixel_format, char depthBits, char stencilBits, bool doubleBuffer, int width, int height):
		Renderer(pFormat, cDepthBits, cStencilBits, doubleBuffer, width, height),
		_colorBuffer(0)
	{
		_bBuf = GP_NEW_ARRAY byte[iWidth * iHeight * colorBytes];
		_clearColor.r = 0;
		_clearColor.g = 0;
		_clearColor.b = 0;

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &_colorBuffer);

		_screenRect = GP_NEW_ARRAY GLfloat[4 * 3];
		_screenRect[0] = 0;
		_screenRect[1] = 0;
		_screenRect[2] = -1;

		_screenRect[3] = iWidth;
		_screenRect[4] = 0;
		_screenRect[5] = -1;

		_screenRect[6] = iWidth;
		_screenRect[7] = iHeight;
		_screenRect[8] = -1;

		_screenRect[9] = 0;
		_screenRect[10] = iHeight;
		_screenRect[11] = -1;

		GLushort _rectIndices[] = {0, 1, 2, 0, 2, 3};

		
	}

	SoftwareRenderer::~SoftwareRenderer()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &_colorBuffer);

		GP_DELETE_ARRAY _bBuf;
		GP_DELETE_ARRAY _screenRect;;
	}

	bool SoftwareRenderer::initContent()
	{
		if(_bBuf)
		{
			return true;
		}else{
			return false;
		}
	}

	void SoftwareRenderer::drawPoint(Point2i &v, Color3b &color)
	{
		if(v.x >= 0 && v.x < iWidth && v.y >= 0 && v.y < iHeight){
			int pos = (iWidth * v.y + v.x) * colorBytes;
			if(_bBuf){
				_bBuf[pos] = color.r;
				_bBuf[pos + 1] = color.g;
				_bBuf[pos + 2] = color.b;
			}
		}
	}

	void SoftwareRenderer::drawPoint(int x, int y, Color3b &color)
	{
		if(x >= 0 && x < iWidth && y >= 0 && y < iHeight){
			int pos = (iWidth * y + x) * colorBytes;
			if(_bBuf){
				_bBuf[pos] = color.r;
				_bBuf[pos + 1] = color.g;
				_bBuf[pos + 2] = color.b;
			}
		}
	}

	void SoftwareRenderer::drawLine(int x0, int y0, int x1, int y1, Color3b &color)
	{
		int dx = x1 - x0;
		int dy = y1 - y0;
		if(dx == 0 && dy == 0)
		{
			drawPoint(x0, y0, color);
			return;
		}
		int change_x = 1;
		int change_y = 1;
		int abs_dx = dx;
		int abs_dy = dy;
		if(dx < 0)
		{
			change_x = -1;
			abs_dx = -dx;
		}
		if(dy < 0)
		{
			change_y = -1;
			abs_dy = -dy;
		}

		int delta = 0;
		int cur_x = x0;
		int cur_y = y0;
		if(abs_dy >= abs_dx)	//change y
		{
			while (cur_y != y1)
			{
				drawPoint(cur_x, cur_y, color);
				cur_y += change_y;
				delta += abs_dx;
				if(delta >= abs_dy)
				{
					cur_x += change_x;
					delta -= abs_dy;
				}
			}
		}
		else
		{
			while (cur_x != x1)
			{
				drawPoint(cur_x, cur_y, color);
				cur_x += change_x;
				delta += abs_dy;
				if(delta >= abs_dx)
				{
					cur_y += change_y;
					delta -= abs_dx;
				}
			}
		}
	}

	void SoftwareRenderer::drawLine(Point2i &v0, Point2i &v1, Color3b &color)
	{
		drawLine(v0.x, v0.y, v1.x, v1.y, color);
	}
	
	void SoftwareRenderer::drawTriangleSolid(int x0, int y0, int x1, int y1, int x2, int y2, Color3b &color)
	{
		int tmp_x = 0;
		int tmp_y = 0;
		//sort with y coord
		if(y1 < y0) {
			tmp_x = x0;
			tmp_y = y0;
			x0 = x1;
			y0 = y1;
			x1 = tmp_x;
			y1 = tmp_y;
		}
		if(y2 < y0) {
			tmp_x = x0;
			tmp_y = y0;
			x0 = x2;
			y0 = y2;
			x2 = tmp_x;
			y2 = tmp_y;
		}
		if(y2 < y1) {
			tmp_x = x1;
			tmp_y = y1;
			x1 = x2;
			y1 = y2;
			x2 = tmp_x;
			y2 = tmp_y;
		}
		//end sort
		//draw down triangle
		if(y0 != y1 && y0 != y2) {
			float dx01 = (float)(x1 - x0) / (y1 - y0);
			float dx02 = (float)(x2 - x0) / (y2 - y0);
			float start_x = x0;
			float end_x = x0;
			if(dx01 > dx02)
			{
				float tmp = dx01;
				dx01 = dx02;
				dx02 = tmp;
			}
			int start_y = y0;
			while(start_y <= y1)
			{
				int iStartX = d_ceil(start_x);
				int iEndX = d_ceil(end_x);
				for (int x = iStartX; x < iEndX; ++x)
				{
					drawPoint(x, start_y, color);
				}
				start_x += dx01;
				end_x += dx02;
				++start_y;
			}
		}
		//draw top triangle
		if(y2 != y0 && y2 != y1) 
		{
			float dx02 = (float)(x2 - x0) / (y2 - y0);
			float dx12 = (float)(x2 - x1) / (y2 - y1); 
			int start_y = y2;
			float start_x = x2;
			if(dx02 < dx12)
			{
				float tmp = dx02;
				dx02 = dx12;
				dx12 = tmp;
			}
			float end_x = x2;
			while (start_y > y1)
			{
				int iStartX = d_ceil(start_x);
				int iEndX = d_ceil(end_x);
				for (int x = iStartX; x < iEndX; ++x)
				{
					drawPoint(x, start_y, color);
				}
				start_x -= dx02;
				end_x -= dx12;
				--start_y;
			}
		}
	}
	
	/*
	void SoftwareRenderer::drawTriangleSolid(int x0, int y0, int x1, int y1, int x2, int y2, Color3b &color)
	{	
		int tmp_x = 0;
		int tmp_y = 0;
		//sort with y coord
		if(y1 < y0) {
			tmp_x = x0;
			tmp_y = y0;
			x0 = x1;
			y0 = y1;
			x1 = tmp_x;
			y1 = tmp_y;
		}
		if(y2 < y0) {
			tmp_x = x0;
			tmp_y = y0;
			x0 = x2;
			y0 = y2;
			x2 = tmp_x;
			y2 = tmp_y;
		}
		if(y2 < y1) {
			tmp_x = x1;
			tmp_y = y1;
			x1 = x2;
			y1 = y2;
			x2 = tmp_x;
			y2 = tmp_y;
		}
		//end sort
		int start_y = y0;
		float start_x = x0;
		float end_x = x0;

		float start_dx0 = ((float)(x1 - x0)) / (y1 - y0);
		float end_dx0 = ((float)(x2 - x0)) / (y2 - y0);
		float start_dx1 = ((float)(x2 - x1)) / (y2 - y1);
		float end_dx1 = end_dx0;

		if(start_dx0 > end_dx0)
		{
			float tmp = start_dx0;
			start_dx0 = end_dx0;
			end_dx0 = tmp;
			
			tmp = start_dx1;
			start_dx1 = end_dx1;
			end_dx1 = tmp;
		}
		
		if(y0 != y1 && y2 != y0)
		{
			while(start_y <= y1)
			{
				int iStartX = d_ceil(start_x);
				int iEndX = d_ceil(end_x);
				for (int x = iStartX; x < iEndX; ++x)
				{
					drawPoint(x, start_y, color);
				}
				start_x += start_dx0;
				end_x += end_dx0;
				start_y += 1;
			}
			start_x -= start_dx0;
			end_x -= end_dx0;
		}
		if(y2 != y1 && y2 != y1)
		{
			while(start_y <= y2)
			{
				int iStartX = d_ceil(start_x);
				int iEndX = d_ceil(end_x);
				for (int x = iStartX; x < iEndX; ++x)
				{
					drawPoint(x, start_y, color);
				}
				start_x += start_dx1;
				end_x += end_dx1;
				start_y += 1;
			}
		}
	}*/
	
	void SoftwareRenderer::drawTriangleSolid(Point2f &v0, Point2f &v1, Point2f &v2, Color3b &color)
	{
		drawTriangleSolid(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, color);
	}
	
	void SoftwareRenderer::setClearColor(const ColorRGBA &color)
	{
		_clearColor.r = (byte)(255 * color.r);
		_clearColor.g = (byte)(255 * color.g);
		_clearColor.b = (byte)(255 * color.b);
	}

	void SoftwareRenderer::clearBuffer()
	{
		int pixelCount = iHeight * iWidth;
		byte *pos = _bBuf;
		unsigned int color = (255 << 24) | (255 << 16) |(255 << 8) | 255;
		for(int idx = 0; idx < pixelCount; ++idx){
			*pos = _clearColor.r;
			*(pos + 1) =  _clearColor.g;
			*(pos + 2) =  _clearColor.b;
			pos +=  colorBytes;
		}
	}

	void SoftwareRenderer::swapBuffer()
	{
		//glBindTexture(GL_TEXTURE_2D, _colorBuffer);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _bBuf);

		glWindowPos2d(0, 0);
		glDrawPixels(iWidth,iHeight, GL_RGB, GL_UNSIGNED_BYTE, _bBuf);
	}

	void SoftwareRenderer::resizeViewPort(int width, int height)
	{
		iWidth = width;
		iHeight = height;

		GP_DELETE_ARRAY _bBuf;
		_bBuf = GP_NEW_ARRAY byte[iWidth * iHeight * colorBytes];
	}
}