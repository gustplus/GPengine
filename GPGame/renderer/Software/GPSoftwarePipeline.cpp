#include <stdio.h>

#include "GPSoftwarePipeline.h"
#include "gp.h"
#include "gpmath.h"
#include "GPGLShader.h"

namespace GPEngine3D{
	// float sqrt[100] = 
	// {
	//  	1, 1, 1, 1, 2, 2, 2, 2, 2, 2
	// };

	SoftwarePipeline::SoftwarePipeline(PixelFormat pixel_format, char depthBits, char stencilBits, bool doubleBuffer, int width, int height):
		Renderer(pFormat, cDepthBits, cStencilBits, doubleBuffer, width, height),
		_fDepthBuffer(nullptr),
		_attr(ATTR_FLAG::COLOR_BUFFER),
		_cullMode(CCW)
	{
		_bColorBuffer = GP_NEW_ARRAY(byte, iWidth * iHeight * colorBytes);
		_clearColor.r = 0;
		_clearColor.g = 0;
		_clearColor.b = 0;

		iBottomY = 0;
		iLeftX = 0;
		iTopY = iBottomY + iHeight - 1;
		iRightX = iLeftX + iWidth - 1;

		GLShaderPtr vShader = new GLShader("Shaders/GLSL/basic.vert", ShaderType::VERTEX_SHADER);
		GLShaderPtr fShader = new GLShader("Shaders/GLSL/basic.frag", ShaderType::FRAG_SHADER);
		_program.attachShader(vShader);
		_program.attachShader(fShader);
		_program.link();


		glGenVertexArrays(1, &_screenRect);
		glBindVertexArray(_screenRect);
		GLuint attriPos;
		GLuint attriTexCoord;
		uniformMat = glGetUniformLocation(_program, "uMVPMatrix");
		attriPos = glGetAttribLocation(_program, "aPosition");
		attriTexCoord = glGetAttribLocation(_program, "aTexCoor");
		glGenBuffers(3, vbos);
		GLfloat vertices[] = {0.0f, 0.0f, 0.0f,
							  width, 0.0f, 0.0f,
							  width, height, 0.0f,
							  0.0f, height, 0.0f};
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); 
		glEnableVertexAttribArray(attriPos);
		glVertexAttribPointer(attriPos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET_BYTE(0));
		GLfloat texCoords[] = {0.0f, 0.0f,
							   1.0f, 0.0f,
							   1.0f, 1.0f,
							   0.0f, 1.0f};
		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(attriTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET_BYTE(0));
		glEnableVertexAttribArray(attriTexCoord);

		GLushort indices[] = {0, 1, 2, 0, 2, 3};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _screenTex);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);  
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);  

		_texIdx = glGetUniformLocation(_program, "sTexture");

		mv.orthf(0,width, 0, height, -10, 10);
		//mv.frustum(-0.01, 0.01, -0.006, 0.006, 0.01, 1000);
	}

	SoftwarePipeline::~SoftwarePipeline()
	{
		GP_SAFE_DELETE_ARRAY(_bColorBuffer);
		GP_SAFE_DELETE_ARRAY(_fDepthBuffer);
	}

	bool SoftwarePipeline::initContent()
	{
		if(_bColorBuffer)
		{
			return true;
		}else{
			return false;
		}
	}

	void SoftwarePipeline::drawPoint(const Point2i &v, const Color3b &color)
	{
		if(v.x >= 0 && v.x < iWidth && v.y >= 0 && v.y < iHeight){
			int pos = (iWidth * v.y + v.x) * colorBytes;
			if(_bColorBuffer){
				_bColorBuffer[pos] = color.r;
				_bColorBuffer[pos + 1] = color.g;
				_bColorBuffer[pos + 2] = color.b;
			}
		}
	}

	void SoftwarePipeline::drawPoint(int x, int y, const Color3b &color)
	{
		if(x >= 0 && x < iWidth && y >= 0 && y < iHeight){
			int pos = (iWidth * y + x) * colorBytes;
			if(_bColorBuffer){
				_bColorBuffer[pos] = color.r;
				_bColorBuffer[pos + 1] = color.g;
				_bColorBuffer[pos + 2] = color.b;
			}
		}
	}

	void SoftwarePipeline::drawLine(int x0, int y0, int x1, int y1, const Color3b &color)
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
		if(dx < 0)
		{
			change_x = -1;
			dx = -dx;
		}
		if(dy < 0)
		{
			change_y = -1;
			dy = -dy;
		}
		int dx2 = dx << 1;
		int dy2 = dy << 1;

		int delta;
		int cur_x = x0;
		int cur_y = y0;
		if(dy >= dx)	//change y
		{
			delta = dx2 - dy;
			while (cur_y != y1)
			{
				drawPoint(cur_x, cur_y, color);
				cur_y += change_y;
				if(delta >= 0)
				{
					cur_x += change_x;
					delta -= dy2;
				}
				delta += dx2;
			}
		}
		else
		{
			delta = dy2 - dx;
			while (cur_x != x1)
			{
				drawPoint(cur_x, cur_y, color);
				cur_x += change_x;
				if(delta >= 0)
				{
					cur_y += change_y;
					delta -= dx2;
				}
				delta += dy2;
			}
		}
	}

	void SoftwarePipeline::drawTriangleSolidFlat(const PolyTriangle &triangle)
	{
		const Vertex &v0 = triangle.tranList[0];
		const Vertex &v1 = triangle.tranList[1];
		const Vertex &v2 = triangle.tranList[2];

		drawTriangleSolidFlat(v0, v1, v2);
	}

	void SoftwarePipeline::drawTriangleSolidFlat(const Vertex &v0, const Vertex & v1, const Vertex &v2)
	{
		const Color4b &color = v2.color;
		int idx0 = 0;
		int idx1 = 1;
		int idx2 = 2;

		int xArr[3] = {(int)(v0.p.x + 0.5f), (int)(v1.p.x + 0.5f), (int)(v2.p.x + 0.5f)};
		int yArr[3] = {(int)(v0.p.y + 0.5f), (int)(v1.p.y + 0.5f), (int)(v2.p.y + 0.5f)};
		float zArr[3] = {v0.p.w, v1.p.w, v2.p.w};

		if((xArr[0] == xArr[1] && xArr[1] == xArr[2]) || (yArr[0] == yArr[1] && yArr[1] == yArr[2]))
		{
			return;
		}
		int tmp_idx = 0;
		//sort v0, v1, v2 by y value
		if(yArr[idx1] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx1;
			idx1 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx2;
			idx2 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx1]) {
			tmp_idx = idx1;
			idx1 = idx2;
			idx2 = tmp_idx;
		}//end sort

		if ( yArr[idx2] < iBottomY || yArr[idx0] > iTopY ||
    		(xArr[idx0] < iLeftX && xArr[idx1] < iLeftX && xArr[idx2] < iLeftX) ||
    		(xArr[idx0] > iRightX && xArr[idx1] > iRightX && xArr[idx2] > iRightX) )
		{
			return;
		}

		int bytesPerRow = iWidth * colorBytes;
		//tmp interpolation values
		float zi;

		int mid_y = yArr[idx1];
		int yb = yArr[idx0];
		int yt = yArr[idx2];

		int dy;

		byte *colorBuf;
		float *depthBuf;

		//draw down triangle
		if(yArr[idx0] != yArr[idx1] && mid_y >= iBottomY && yb <= iTopY) {
			//sort v1, v2 by dx value
			int idxl = idx1;
			int idxr = idx2;
			float inv_dyl = 1.0f / (yArr[idxl] - yArr[idx0]);
			float inv_dyr = 1.0f / (yArr[idxr] - yArr[idx0]);
			float dxl = (float)(xArr[idxl] - xArr[idx0]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx0]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx0] <= iRightX) && (xArr[idxr] >= iLeftX || xArr[idx0] >= iLeftX) )
			{
				//use for interpolation
				float dzl = (zArr[idxl] - zArr[idx0]) * inv_dyl;			
				float dzr = (zArr[idxr] - zArr[idx0]) * inv_dyr;
	
				float xl = xArr[idx0];
				float xr = xArr[idx0];
				float zl = zArr[idx0];
				float zr = zArr[idx0];

				dy = iBottomY - yb;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					yb = iBottomY;
				}
				int end_y = mid_y > iTopY ? iTopY : mid_y;
				while(yb <= end_y)
				{
					int dx = xr - xl;
					if(dx > 0){
						zi = zl;

						float inv_dx = 1.0f / dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yb * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yb * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{	
							if(zi > *depthBuf){
								*colorBuf = color.r;
								*(colorBuf + 1) = color.g;
								*(colorBuf + 2) = color.b;
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					++yb;
				}
			}
		}
		//draw top triangle
		if(yArr[idx2] != yArr[idx1] && mid_y <= iTopY && yt >= iBottomY) {
			//sort v0, v1 by dx value
			int idxl = idx0;
			int idxr = idx1;
			float inv_dyl = 1.0f / (yArr[idx2] - yArr[idxl]);
			float inv_dyr = 1.0f / (yArr[idx2] - yArr[idxr]);
			float dxl = (float)(xArr[idxl] - xArr[idx2]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx2]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx2] <= iRightX) && (xArr[idx2] >= iLeftX || xArr[idxr] >= iLeftX) )
			{
				//use for interpolation
				float dzl = (zArr[idxl] - zArr[idx2]) * inv_dyl;
				float dzr = (zArr[idxr] - zArr[idx2]) * inv_dyr;

				float xl = xArr[idx2];
				float xr = xArr[idx2];
				float zl = zArr[idx2];
				float zr = zArr[idx2];

				dy = yt - iTopY;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					yt = iTopY;
				}
				int end_y = mid_y > iBottomY ? mid_y : iBottomY;
				while (yt >= end_y)
				{
					int dx = xr - xl;
					if(dx > 0)
					{
						zi = zl;

						float inv_dx = 1.0f / dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yt * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yt * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{
							if(zi > *depthBuf){
								float inv_z = 1.0f / zi;
								*colorBuf = color.r;
								*(colorBuf + 1) = color.g;
								*(colorBuf + 2) = color.b;
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					--yt;
				}
			}
		}
	}

	void SoftwarePipeline::drawTriangleSolidColor(const PolyTriangle &triangle)
	{
		const Vertex &v0 = triangle.tranList[0];
		const Vertex &v1 = triangle.tranList[1];
		const Vertex &v2 = triangle.tranList[2];

		drawTriangleSolidColor(v0, v1, v2);
	}

	void SoftwarePipeline::drawTriangleSolidColor(const Vertex &v0, const Vertex & v1, const Vertex &v2)
	{
		int idx0 = 0;
		int idx1 = 1;
		int idx2 = 2;

		int xArr[3] = {(int)(v0.p.x + 0.5f), (int)(v1.p.x + 0.5f), (int)(v2.p.x + 0.5f)};
		int yArr[3] = {(int)(v0.p.y + 0.5f), (int)(v1.p.y + 0.5f), (int)(v2.p.y + 0.5f)};
		float zArr[3] = {v0.p.w, v1.p.w, v2.p.w};
		float uArr[3] = {v0.uv.x, v1.uv.x, v2.uv.x};
		float vArr[3] = {v0.uv.y, v1.uv.y, v2.uv.y};

		float rArr[3] = {v0.color.r * zArr[0], v1.color.r * zArr[1], v2.color.r * zArr[2]};
		float gArr[3] = {v0.color.g * zArr[0], v1.color.g * zArr[1], v2.color.g * zArr[2]};
		float bArr[3] = {v0.color.b * zArr[0], v1.color.b * zArr[1], v2.color.b * zArr[2]};
		float aArr[3] = {v0.color.a * zArr[0], v1.color.a * zArr[1], v2.color.a * zArr[2]};

		if((xArr[0] == xArr[1] && xArr[1] == xArr[2]) || (yArr[0] == yArr[1] && yArr[1] == yArr[2]))
		{
			return;
		}
		int tmp_idx = 0;
		//sort v0, v1, v2 by y value
		if(yArr[idx1] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx1;
			idx1 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx2;
			idx2 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx1]) {
			tmp_idx = idx1;
			idx1 = idx2;
			idx2 = tmp_idx;
		}//end sort

		if ( yArr[idx2] < iBottomY || yArr[idx0] > iTopY ||
    		(xArr[idx0] < iLeftX && xArr[idx1] < iLeftX && xArr[idx2] < iLeftX) ||
    		(xArr[idx0] > iRightX && xArr[idx1] > iRightX && xArr[idx2] > iRightX) )
		{
			return;
		}

		int bytesPerRow = iWidth * colorBytes;
		//tmp interpolation values
		float ui;
		float vi;
		float ri;
		float gi;
		float bi;
		float ai;
		float zi;

		int mid_y = yArr[idx1];
		int yb = yArr[idx0];
		int yt = yArr[idx2];

		int dy;

		byte *colorBuf;
		float *depthBuf;

		//draw down triangle
		if(yArr[idx0] != yArr[idx1] && mid_y >= iBottomY && yb <= iTopY) {
			//sort v1, v2 by dx value
			int idxl = idx1;
			int idxr = idx2;
			float inv_dyl = 1.0f / (yArr[idxl] - yArr[idx0]);
			float inv_dyr = 1.0f / (yArr[idxr] - yArr[idx0]);
			float dxl = (float)(xArr[idxl] - xArr[idx0]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx0]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx0] <= iRightX) && (xArr[idxr] >= iLeftX || xArr[idx0] >= iLeftX) )
			{
				//use for interpolation
				float dzl = (zArr[idxl] - zArr[idx0]) * inv_dyl;
				float dzr = (zArr[idxr] - zArr[idx0]) * inv_dyr;

				float dul = (uArr[idxl] - uArr[idx0]) * inv_dyl;
				float dvl = (vArr[idxl] - vArr[idx0]) * inv_dyl;
				float dur = (uArr[idxr] - uArr[idx0]) * inv_dyr;
				float dvr = (vArr[idxr] - vArr[idx0]) * inv_dyr;

				float drl = (rArr[idxl] - rArr[idx0]) * inv_dyl;
				float dgl = (gArr[idxl] - gArr[idx0]) * inv_dyl;
				float dbl = (bArr[idxl] - bArr[idx0]) * inv_dyl;
				float dal = (aArr[idxl] - aArr[idx0]) * inv_dyl;
				float drr = (rArr[idxr] - rArr[idx0]) * inv_dyr;
				float dgr = (gArr[idxr] - gArr[idx0]) * inv_dyr;
				float dbr = (bArr[idxr] - bArr[idx0]) * inv_dyr;
				float dar = (aArr[idxr] - aArr[idx0]) * inv_dyr;

				float xl = xArr[idx0];
				float xr = xArr[idx0];
				float zl = zArr[idx0];
				float zr = zArr[idx0];
				float ul = uArr[idx0];
				float ur = uArr[idx0];
				float vl = vArr[idx0];
				float vr = vArr[idx0];
				float rl = rArr[idx0];
				float gl = gArr[idx0];
				float bl = bArr[idx0];
				float al = aArr[idx0];
				float rr = rArr[idx0];
				float gr = gArr[idx0];
				float br = bArr[idx0];
				float ar = aArr[idx0];

				dy = iBottomY - yb;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					ul += dul * dy;
					ur += dur * dy;
					vl += dvl * dy;
					vr += dvr * dy;
					rl += drl * dy;
					rr += drr * dy;
					gl += dgl * dy;
					gr += dgr * dy;
					bl += dbl * dy;
					br += dbr * dy;
					al += dal * dy;
					ar += dar * dy;
					yb = iBottomY;
				}
				int end_y = mid_y > iTopY ? iTopY : mid_y;
				while(yb <= end_y)
				{
					int dx = xr - xl;
					if(dx > 0){
						ui = ul;
						vi = vl;
						ri = rl;
						gi = gl;
						bi = bl;
						ai = al;
						zi = zl;

						float inv_dx = 1.0f / dx;
						float du = (ur - ul) * inv_dx;
						float dv = (vr - vl) * inv_dx;
						float dr = (rr - rl) * inv_dx;
						float dg = (gr - gl) * inv_dx;
						float db = (br - bl) * inv_dx;
						float da = (ar - al) * inv_dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							ri += dx * dr;
							gi += dx * dg;
							bi += dx * db;
							ai += dx * da;
							ui += dx * du;
							vi += dx * dv;
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yb * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yb * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{	
							if(zi > *depthBuf){
								float inv_z = 1.0f / zi;
								*colorBuf = (byte)(ri * inv_z);
								*(colorBuf + 1) = (byte)(gi * inv_z);
								*(colorBuf + 2) = (byte)(bi * inv_z);
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							ri += dr;
							gi += dg;
							bi += db;
							ai += da;
							ui += du;
							vi += dv;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					ul += dul;
					ur += dur;
					vl += dvl;
					vr += dvr;
					rl += drl;
					rr += drr;
					gl += dgl;
					gr += dgr;
					bl += dbl;
					br += dbr;
					al += dal;
					ar += dar;
					++yb;
				}
			}
		}
		//draw top triangle
		if(yArr[idx2] != yArr[idx1] && mid_y <= iTopY && yt >= iBottomY) {
			//sort v0, v1 by dx value
			int idxl = idx0;
			int idxr = idx1;
			float inv_dyl = 1.0f / (yArr[idx2] - yArr[idxl]);
			float inv_dyr = 1.0f / (yArr[idx2] - yArr[idxr]);
			float dxl = (float)(xArr[idxl] - xArr[idx2]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx2]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx2] <= iRightX) && (xArr[idx2] >= iLeftX || xArr[idxr] >= iLeftX) )
			{
				//use for interpolation
				float dzl = (zArr[idxl] - zArr[idx2]) * inv_dyl;
				float dzr = (zArr[idxr] - zArr[idx2]) * inv_dyr;

				float dul = (uArr[idxl] - uArr[idx2]) * inv_dyl;
				float dvl = (vArr[idxl] - vArr[idx2]) * inv_dyl;
				float dur = (uArr[idxr] - uArr[idx2]) * inv_dyr;
				float dvr = (vArr[idxr] - vArr[idx2]) * inv_dyr;

				float drl = (rArr[idxl] - rArr[idx2]) * inv_dyl;
				float dgl = (gArr[idxl] - gArr[idx2]) * inv_dyl;
				float dbl = (bArr[idxl] - bArr[idx2]) * inv_dyl;
				float dal = (aArr[idxl] - aArr[idx2]) * inv_dyl;
				float drr = (rArr[idxr] - rArr[idx2]) * inv_dyr;
				float dgr = (gArr[idxr] - gArr[idx2]) * inv_dyr;
				float dbr = (bArr[idxr] - bArr[idx2]) * inv_dyr;
				float dar = (aArr[idxr] - aArr[idx2]) * inv_dyr;

				float xl = xArr[idx2];
				float xr = xArr[idx2];
				float zl = zArr[idx2];
				float zr = zArr[idx2];
				float ul = uArr[idx2];
				float ur = uArr[idx2];
				float vl = vArr[idx2];
				float vr = vArr[idx2];
				float rl = rArr[idx2];
				float gl = gArr[idx2];
				float bl = bArr[idx2];
				float al = aArr[idx2];
				float rr = rArr[idx2];
				float gr = gArr[idx2];
				float br = bArr[idx2];
				float ar = aArr[idx2];

				dy = yt - iTopY;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					ul += dul * dy;
					ur += dur * dy;
					vl += dvl * dy;
					vr += dvr * dy;
					rl += drl * dy;
					rr += drr * dy;
					gl += dgl * dy;
					gr += dgr * dy;
					bl += dbl * dy;
					br += dbr * dy;
					al += dal * dy;
					ar += dar * dy;
					yt = iTopY;
				}
				int end_y = mid_y > iBottomY ? mid_y : iBottomY;
				while (yt >= end_y)
				{
					int dx = xr - xl;
					if(dx > 0){
						ui = ul;
						vi = vl;
						ri = rl;
						gi = gl;
						bi = bl;
						ai = al;
						zi = zl;

						float inv_dx = 1.0f / dx;
						float du = (ur - ul) * inv_dx;
						float dv = (vr - vl) * inv_dx;
						float dr = (rr - rl) * inv_dx;
						float dg = (gr - gl) * inv_dx;
						float db = (br - bl) * inv_dx;
						float da = (ar - al) * inv_dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							ri += dx * dr;
							gi += dx * dg;
							bi += dx * db;
							ai += dx * da;
							ui += dx * du;
							vi += dx * dv;
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yt * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yt * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{
							if(zi > *depthBuf){
								float inv_z = 1.0f / zi;
								*colorBuf = (byte)(ri * inv_z);
								*(colorBuf + 1) = (byte)(gi * inv_z);
								*(colorBuf + 2) = (byte)(bi * inv_z);
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							ri += dr;
							gi += dg;
							bi += db;
							ai += da;
							ui += du;
							vi += dv;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					ul += dul;
					ur += dur;
					vl += dvl;
					vr += dvr;
					rl += drl;
					rr += drr;
					gl += dgl;
					gr += dgr;
					bl += dbl;
					br += dbr;
					al += dal;
					ar += dar;
					--yt;
				}
			}
		}
	}

	void SoftwarePipeline::drawTriangleSolidTexNearest(const PolyTriangle &triangle, const Texture2D &tex)
	{
		const Vertex &v0 = triangle.tranList[0];
		const Vertex &v1 = triangle.tranList[1];
		const Vertex &v2 = triangle.tranList[2];

		drawTriangleSolidTexNearest(v0, v1, v2, tex);
	}
	
	void SoftwarePipeline::drawTriangleSolidTexNearest(const Vertex &v0, const Vertex & v1, const Vertex &v2, const Texture2D &tex)
	{
		int idx0 = 0;
		int idx1 = 1;
		int idx2 = 2;

		int xArr[3] = {(int)(v0.p.x + 0.5f), (int)(v1.p.x + 0.5f), (int)(v2.p.x + 0.5f)};
		int yArr[3] = {(int)(v0.p.y + 0.5f), (int)(v1.p.y + 0.5f), (int)(v2.p.y + 0.5f)};
		float zArr[3] = {v0.p.w, v1.p.w, v2.p.w};

		float uArr[3] = {v0.uv.x, v1.uv.x, v2.uv.x};
		float vArr[3] = {v0.uv.y, v1.uv.y, v2.uv.y};

		float rArr[3] = {v0.color.r * zArr[0], v1.color.r * zArr[1], v2.color.r * zArr[2]};
		float gArr[3] = {v0.color.g * zArr[0], v1.color.g * zArr[1], v2.color.g * zArr[2]};
		float bArr[3] = {v0.color.b * zArr[0], v1.color.b * zArr[1], v2.color.b * zArr[2]};
		float aArr[3] = {v0.color.a * zArr[0], v1.color.a * zArr[1], v2.color.a * zArr[2]};

		if((xArr[0] == xArr[1] && xArr[1] == xArr[2]) || (yArr[0] == yArr[1] && yArr[1] == yArr[2]))
		{
			return;
		}
		int tmp_idx = 0;
		//sort v0, v1, v2 by y value
		if(yArr[idx1] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx1;
			idx1 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx2;
			idx2 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx1]) {
			tmp_idx = idx1;
			idx1 = idx2;
			idx2 = tmp_idx;
		}//end sort

		if ( yArr[idx2] < iBottomY || yArr[idx0] > iTopY ||
    		(xArr[idx0] < iLeftX && xArr[idx1] < iLeftX && xArr[idx2] < iLeftX) ||
    		(xArr[idx0] > iRightX && xArr[idx1] > iRightX && xArr[idx2] > iRightX) )
		{
			return;
		}
		
		int bytesPerRow = iWidth * colorBytes;
		//tmp interpolation values
		float ui;
		float vi;
		float ri;
		float gi;
		float bi;
		float ai;
		float zi;

		int mid_y = yArr[idx1];
		int yb = yArr[idx0];
		int yt = yArr[idx2];

		int dy;

		byte *colorBuf;
		float *depthBuf;

		//calaculate the mipmap level
		//find the left x nd right x
		int leftIdx = idx0;
		int rightIdx = idx0;
		leftIdx = xArr[leftIdx] > xArr[idx1] ? idx1 : leftIdx;
		leftIdx = xArr[leftIdx] > xArr[idx2] ? idx2 : leftIdx;
		rightIdx = xArr[rightIdx] < xArr[idx1] ? idx1 : rightIdx;
		rightIdx = xArr[rightIdx] < xArr[idx2] ? idx2 : rightIdx;
		float midLen = (xArr[leftIdx] - xArr[rightIdx]);
		float texMidLen = (uArr[leftIdx] / zArr[leftIdx] - uArr[rightIdx] / zArr[rightIdx]) * tex.getWidth();
		int mipmapLv = 0;
		if(midLen != 0)
			mipmapLv = int((texMidLen / midLen) + 0.5f);
		int maxMipmapLv = tex.getMaxMipmapLevel();
		mipmapLv = maxMipmapLv < mipmapLv ? maxMipmapLv : mipmapLv;
		mipmapLv = 0 > mipmapLv ? 0 : mipmapLv;
		Image *img = tex.getImage(mipmapLv);
		byte *imgBuf = img->getData();
		int texWidth = img->getWidth();
		int texHeight = img->getHeight();
		//draw down triangle
		if(yArr[idx0] != yArr[idx1] && mid_y >= iBottomY && yb <= iTopY) {
			//sort v1, v2 by dx value
			int idxl = idx1;
			int idxr = idx2;
			float inv_dyl = 1.0f / (yArr[idxl] - yArr[idx0]);
			float inv_dyr = 1.0f / (yArr[idxr] - yArr[idx0]);
			float dxl = (float)(xArr[idxl] - xArr[idx0]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx0]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx0] <= iRightX) && (xArr[idxr] >= iLeftX || xArr[idx0] >= iLeftX) )
			{
				//use for interpolation
				float dzl = (zArr[idxl] - zArr[idx0]) * inv_dyl;
				float dzr = (zArr[idxr] - zArr[idx0]) * inv_dyr;

				float dul = (uArr[idxl] - uArr[idx0]) * inv_dyl;
				float dvl = (vArr[idxl] - vArr[idx0]) * inv_dyl;
				float dur = (uArr[idxr] - uArr[idx0]) * inv_dyr;
				float dvr = (vArr[idxr] - vArr[idx0]) * inv_dyr;

				float drl = (rArr[idxl] - rArr[idx0]) * inv_dyl;
				float dgl = (gArr[idxl] - gArr[idx0]) * inv_dyl;
				float dbl = (bArr[idxl] - bArr[idx0]) * inv_dyl;
				float dal = (aArr[idxl] - aArr[idx0]) * inv_dyl;
				float drr = (rArr[idxr] - rArr[idx0]) * inv_dyr;
				float dgr = (gArr[idxr] - gArr[idx0]) * inv_dyr;
				float dbr = (bArr[idxr] - bArr[idx0]) * inv_dyr;
				float dar = (aArr[idxr] - aArr[idx0]) * inv_dyr;

				float xl = xArr[idx0];
				float xr = xArr[idx0];
				float zl = zArr[idx0];
				float zr = zArr[idx0];
				float ul = uArr[idx0];
				float ur = uArr[idx0];
				float vl = vArr[idx0];
				float vr = vArr[idx0];
				float rl = rArr[idx0];
				float gl = gArr[idx0];
				float bl = bArr[idx0];
				float al = aArr[idx0];
				float rr = rArr[idx0];
				float gr = gArr[idx0];
				float br = bArr[idx0];
				float ar = aArr[idx0];

				dy = iBottomY - yb;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					ul += dul * dy;
					ur += dur * dy;
					vl += dvl * dy;
					vr += dvr * dy;
					rl += drl * dy;
					rr += drr * dy;
					gl += dgl * dy;
					gr += dgr * dy;
					bl += dbl * dy;
					br += dbr * dy;
					al += dal * dy;
					ar += dar * dy;
					yb = iBottomY;
				}
				ul *= (texWidth - 1);
				ur *= (texWidth - 1);
				vl *= (texHeight - 1);
				vr *= (texHeight - 1);
				dul *= (texWidth - 1);
				dur *= (texWidth - 1);
				dvl *= (texHeight - 1);
				dvr *= (texHeight - 1);

				int end_y = mid_y > iTopY ? iTopY : mid_y;
				while(yb <= end_y)
				{
					int dx = xr - xl;
					if(dx > 0){
						ui = ul;
						vi = vl;
						ri = rl;
						gi = gl;
						bi = bl;
						ai = al;
						zi = zl;

						float inv_dx = 1.0f / dx;
						float du = (ur - ul) * inv_dx;
						float dv = (vr - vl) * inv_dx;
						float dr = (rr - rl) * inv_dx;
						float dg = (gr - gl) * inv_dx;
						float db = (br - bl) * inv_dx;
						float da = (ar - al) * inv_dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							ri += dx * dr;
							gi += dx * dg;
							bi += dx * db;
							ai += dx * da;
							ui += dx * du;
							vi += dx * dv;
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yb * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yb * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{	
							if(zi > *depthBuf){
								float inv_z = 1.0f / zi;
								int u = (int)(ui * inv_z + 0.5f);
								int v = (int)(vi * inv_z + 0.5f);
								byte *pImg = imgBuf + (v * texWidth + u) * 3;
								*colorBuf = *pImg;
								*(colorBuf + 1) = *(pImg + 1);
								*(colorBuf + 2) = *(pImg + 2);
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							ri += dr;
							gi += dg;
							bi += db;
							ai += da;
							ui += du;
							vi += dv;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					ul += dul;
					ur += dur;
					vl += dvl;
					vr += dvr;
					rl += drl;
					rr += drr;
					gl += dgl;
					gr += dgr;
					bl += dbl;
					br += dbr;
					al += dal;
					ar += dar;
					++yb;
				}
			}
		}
		//draw top triangle
		if(yArr[idx2] != yArr[idx1] && mid_y <= iTopY && yt >= iBottomY) {
			//sort v0, v1 by dx value
			int idxl = idx0;
			int idxr = idx1;
			float inv_dyl = 1.0f / (yArr[idx2] - yArr[idxl]);
			float inv_dyr = 1.0f / (yArr[idx2] - yArr[idxr]);
			float dxl = (float)(xArr[idxl] - xArr[idx2]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx2]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx2] <= iRightX) && (xArr[idx2] >= iLeftX || xArr[idxr] >= iLeftX) )
			{
				//use for interpolation
				float dzl = (zArr[idxl] - zArr[idx2]) * inv_dyl;
				float dzr = (zArr[idxr] - zArr[idx2]) * inv_dyr;

				float dul = (uArr[idxl] - uArr[idx2]) * inv_dyl;
				float dvl = (vArr[idxl] - vArr[idx2]) * inv_dyl;
				float dur = (uArr[idxr] - uArr[idx2]) * inv_dyr;
				float dvr = (vArr[idxr] - vArr[idx2]) * inv_dyr;

				float drl = (rArr[idxl] - rArr[idx2]) * inv_dyl;
				float dgl = (gArr[idxl] - gArr[idx2]) * inv_dyl;
				float dbl = (bArr[idxl] - bArr[idx2]) * inv_dyl;
				float dal = (aArr[idxl] - aArr[idx2]) * inv_dyl;
				float drr = (rArr[idxr] - rArr[idx2]) * inv_dyr;
				float dgr = (gArr[idxr] - gArr[idx2]) * inv_dyr;
				float dbr = (bArr[idxr] - bArr[idx2]) * inv_dyr;
				float dar = (aArr[idxr] - aArr[idx2]) * inv_dyr;

				float xl = xArr[idx2];
				float xr = xArr[idx2];
				float zl = zArr[idx2];
				float zr = zArr[idx2];
				float ul = uArr[idx2];
				float ur = uArr[idx2];
				float vl = vArr[idx2];
				float vr = vArr[idx2];
				float rl = rArr[idx2];
				float gl = gArr[idx2];
				float bl = bArr[idx2];
				float al = aArr[idx2];
				float rr = rArr[idx2];
				float gr = gArr[idx2];
				float br = bArr[idx2];
				float ar = aArr[idx2];

				dy = yt - iTopY;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					ul += dul * dy;
					ur += dur * dy;
					vl += dvl * dy;
					vr += dvr * dy;
					rl += drl * dy;
					rr += drr * dy;
					gl += dgl * dy;
					gr += dgr * dy;
					bl += dbl * dy;
					br += dbr * dy;
					al += dal * dy;
					ar += dar * dy;
					yt = iTopY;
				}

				ul *= (texWidth - 1);
				ur *= (texWidth - 1);
				vl *= (texHeight - 1);
				vr *= (texHeight - 1);
				dul *= (texWidth - 1);
				dur *= (texWidth - 1);
				dvl *= (texHeight - 1);
				dvr *= (texHeight - 1);

				int end_y = mid_y > iBottomY ? mid_y : iBottomY;
				while (yt >= end_y)
				{
					int dx = xr - xl;
					if(dx > 0){
						ui = ul;
						vi = vl;
						ri = rl;
						gi = gl;
						bi = bl;
						ai = al;
						zi = zl;

						float inv_dx = 1.0f / dx;
						float du = (ur - ul) * inv_dx;
						float dv = (vr - vl) * inv_dx;
						float dr = (rr - rl) * inv_dx;
						float dg = (gr - gl) * inv_dx;
						float db = (br - bl) * inv_dx;
						float da = (ar - al) * inv_dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							ri += dx * dr;
							gi += dx * dg;
							bi += dx * db;
							ai += dx * da;
							ui += dx * du;
							vi += dx * dv;
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yt * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yt * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{
							if(zi > *depthBuf){
								float inv_z = 1.0f / zi;
								int u = (int)(ui * inv_z + 0.5f);
								int v = (int)(vi * inv_z + 0.5f);
								byte *pImg = imgBuf + (v * texWidth + u) * 3;
								*colorBuf = *pImg;
								*(colorBuf + 1) = *(pImg + 1);
								*(colorBuf + 2) = *(pImg + 2);
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							ri += dr;
							gi += dg;
							bi += db;
							ai += da;
							ui += du;
							vi += dv;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					ul += dul;
					ur += dur;
					vl += dvl;
					vr += dvr;
					rl += drl;
					rr += drr;
					gl += dgl;
					gr += dgr;
					bl += dbl;
					br += dbr;
					al += dal;
					ar += dar;
					--yt;
				}
			}
		}
	}

	void SoftwarePipeline::drawTriangleSolidTexLinear(const PolyTriangle &triangle, const Texture2D &tex)
	{
		const Vertex &v0 = triangle.tranList[0];
		const Vertex &v1 = triangle.tranList[1];
		const Vertex &v2 = triangle.tranList[2];

		drawTriangleSolidTexLinear(v0, v1, v2, tex);
	}
	
	void SoftwarePipeline::drawTriangleSolidTexLinear(const Vertex &v0, const Vertex & v1, const Vertex &v2, const Texture2D &tex)
	{
		int idx0 = 0;
		int idx1 = 1;
		int idx2 = 2;

		int xArr[3] = {(int)(v0.p.x + 0.5f), (int)(v1.p.x + 0.5f), (int)(v2.p.x + 0.5f)};
		int yArr[3] = {(int)(v0.p.y + 0.5f), (int)(v1.p.y + 0.5f), (int)(v2.p.y + 0.5f)};
		float zArr[3] = {v0.p.w, v1.p.w, v2.p.w};

		float uArr[3] = {v0.uv.x, v1.uv.x, v2.uv.x};
		float vArr[3] = {v0.uv.y, v1.uv.y, v2.uv.y};

		float rArr[3] = {v0.color.r * zArr[0], v1.color.r * zArr[1], v2.color.r * zArr[2]};
		float gArr[3] = {v0.color.g * zArr[0], v1.color.g * zArr[1], v2.color.g * zArr[2]};
		float bArr[3] = {v0.color.b * zArr[0], v1.color.b * zArr[1], v2.color.b * zArr[2]};
		float aArr[3] = {v0.color.a * zArr[0], v1.color.a * zArr[1], v2.color.a * zArr[2]};

		if((xArr[0] == xArr[1] && xArr[1] == xArr[2]) || (yArr[0] == yArr[1] && yArr[1] == yArr[2]))
		{
			return;
		}
		int tmp_idx = 0;
		//sort v0, v1, v2 by y value
		if(yArr[idx1] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx1;
			idx1 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx2;
			idx2 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx1]) {
			tmp_idx = idx1;
			idx1 = idx2;
			idx2 = tmp_idx;
		}//end sort

		if ( yArr[idx2] < iBottomY || yArr[idx0] > iTopY ||
    		(xArr[idx0] < iLeftX && xArr[idx1] < iLeftX && xArr[idx2] < iLeftX) ||
    		(xArr[idx0] > iRightX && xArr[idx1] > iRightX && xArr[idx2] > iRightX) )
		{
			return;
		}

		int bytesPerRow = iWidth * colorBytes;
		//tmp interpolation values
		float ui;
		float vi;
		float ri;
		float gi;
		float bi;
		float ai;
		float zi;

		int mid_y = yArr[idx1];
		int yb = yArr[idx0];
		int yt = yArr[idx2];

		int dy;

		byte *colorBuf;
		float *depthBuf;
		//calaculate the mipmap level
		int leftIdx = idx0;
		int rightIdx = idx0;
		leftIdx = xArr[leftIdx] > xArr[idx1] ? idx1 : leftIdx;
		leftIdx = xArr[leftIdx] > xArr[idx2] ? idx2 : leftIdx;
		rightIdx = xArr[rightIdx] < xArr[idx1] ? idx1 : rightIdx;
		rightIdx = xArr[rightIdx] < xArr[idx2] ? idx2 : rightIdx;
		float midLen = (xArr[leftIdx] - xArr[rightIdx]);
		float texMidLen = (uArr[leftIdx] / zArr[leftIdx] - uArr[rightIdx] / zArr[rightIdx]) * tex.getWidth();
		int mipmapLv = 0;
		if(midLen != 0)
			mipmapLv = int((texMidLen / midLen) + 0.5f);
		int maxMipmapLv = tex.getMaxMipmapLevel();
		mipmapLv = maxMipmapLv < mipmapLv ? maxMipmapLv : mipmapLv;
		mipmapLv = 0 > mipmapLv ? 0 : mipmapLv;
		Image *img = tex.getImage(mipmapLv);
		byte *imgBuf = img->getData();
		int texWidth = img->getWidth();
		int texHeight = img->getHeight();
		//draw down triangle
		if(yArr[idx0] != yArr[idx1] && mid_y >= iBottomY && yb <= iTopY) {
			//sort v1, v2 by dx value
			int idxl = idx1;
			int idxr = idx2;
			float inv_dyl = 1.0f / (yArr[idxl] - yArr[idx0]);
			float inv_dyr = 1.0f / (yArr[idxr] - yArr[idx0]);
			float dxl = (float)(xArr[idxl] - xArr[idx0]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx0]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx0] <= iRightX) && (xArr[idxr] >= iLeftX || xArr[idx0] >= iLeftX) )
			{
				float dzl = (zArr[idxl] - zArr[idx0]) * inv_dyl;
				float dzr = (zArr[idxr] - zArr[idx0]) * inv_dyr;

				float dul = (uArr[idxl] - uArr[idx0]) * inv_dyl;
				float dvl = (vArr[idxl] - vArr[idx0]) * inv_dyl;
				float dur = (uArr[idxr] - uArr[idx0]) * inv_dyr;
				float dvr = (vArr[idxr] - vArr[idx0]) * inv_dyr;

				float drl = (rArr[idxl] - rArr[idx0]) * inv_dyl;
				float dgl = (gArr[idxl] - gArr[idx0]) * inv_dyl;
				float dbl = (bArr[idxl] - bArr[idx0]) * inv_dyl;
				float dal = (aArr[idxl] - aArr[idx0]) * inv_dyl;
				float drr = (rArr[idxr] - rArr[idx0]) * inv_dyr;
				float dgr = (gArr[idxr] - gArr[idx0]) * inv_dyr;
				float dbr = (bArr[idxr] - bArr[idx0]) * inv_dyr;
				float dar = (aArr[idxr] - aArr[idx0]) * inv_dyr;

				float xl = xArr[idx0];
				float xr = xArr[idx0];
				float zl = zArr[idx0];
				float zr = zArr[idx0];
				float ul = uArr[idx0];
				float ur = uArr[idx0];
				float vl = vArr[idx0];
				float vr = vArr[idx0];
				float rl = rArr[idx0];
				float gl = gArr[idx0];
				float bl = bArr[idx0];
				float al = aArr[idx0];
				float rr = rArr[idx0];
				float gr = gArr[idx0];
				float br = bArr[idx0];
				float ar = aArr[idx0];

				dy = iBottomY - yb;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					ul += dul * dy;
					ur += dur * dy;
					vl += dvl * dy;
					vr += dvr * dy;
					rl += drl * dy;
					rr += drr * dy;
					gl += dgl * dy;
					gr += dgr * dy;
					bl += dbl * dy;
					br += dbr * dy;
					al += dal * dy;
					ar += dar * dy;
					yb = iBottomY;
				}
				ul *= (texWidth - 1);
				ur *= (texWidth - 1);
				vl *= (texHeight - 1);
				vr *= (texHeight - 1);
				dul *= (texWidth - 1);
				dur *= (texWidth - 1);
				dvl *= (texHeight - 1);
				dvr *= (texHeight - 1);

				int end_y = mid_y > iTopY ? iTopY : mid_y;
				while(yb <= end_y)
				{
					int dx = xr - xl;
					if(dx > 0){
						ui = ul;
						vi = vl;
						ri = rl;
						gi = gl;
						bi = bl;
						ai = al;
						zi = zl;

						float inv_dx = 1.0f / dx;
						float du = (ur - ul) * inv_dx;
						float dv = (vr - vl) * inv_dx;
						float dr = (rr - rl) * inv_dx;
						float dg = (gr - gl) * inv_dx;
						float db = (br - bl) * inv_dx;
						float da = (ar - al) * inv_dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							ri += dx * dr;
							gi += dx * dg;
							bi += dx * db;
							ai += dx * da;
							ui += dx * du;
							vi += dx * dv;
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yb * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yb * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{	
							if(zi > *depthBuf){
								float inv_z = 1.0f / zi;
								float fu = ui * inv_z;
								float fv = vi * inv_z;
								int u = (int)(fu);
								int v = (int)(fv);
								float d0x = fu - u;
								float d0y = fv - v;
								float d1x = 1.0f - d0x;
								float d1y = 1.0f - d0y;
								byte *pImg = imgBuf + (v * texWidth + u) * 3;
								byte c0r = *pImg;
								byte c0g = *(++pImg);
								byte c0b = *(++pImg);
								byte c1r = *(++pImg);
								byte c1g = *(++pImg);
								byte c1b = *(++pImg);
								pImg += (texWidth * 3) - 5;
								byte c2r = *pImg;
								byte c2g = *(++pImg);
								byte c2b = *(++pImg);
								byte c3r = *(++pImg);
								byte c3g = *(++pImg);
								byte c3b = *(++pImg);

								float a = d1x * d1y;
								float b = d0x * d1y;
								float c = d1x * d0y;
								float d = d0x * d0y;

								*colorBuf = a * c0r + b * c1r + c * c2r + d * c3r;
								*(colorBuf + 1) = a * c0g + b * c1g + c * c2g + d * c3g;
								*(colorBuf + 2) = a * c0b + b * c1b + c * c2b + d * c3b;
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							ri += dr;
							gi += dg;
							bi += db;
							ai += da;
							ui += du;
							vi += dv;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					ul += dul;
					ur += dur;
					vl += dvl;
					vr += dvr;
					rl += drl;
					rr += drr;
					gl += dgl;
					gr += dgr;
					bl += dbl;
					br += dbr;
					al += dal;
					ar += dar;
					++yb;
				}
			}
		}
		//draw top triangle
		if(yArr[idx2] != yArr[idx1] && mid_y <= iTopY && yt >= iBottomY) {
			//sort v0, v1 by dx value
			int idxl = idx0;
			int idxr = idx1;
			float inv_dyl = 1.0f / (yArr[idx2] - yArr[idxl]);
			float inv_dyr = 1.0f / (yArr[idx2] - yArr[idxr]);
			float dxl = (float)(xArr[idxl] - xArr[idx2]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx2]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx2] <= iRightX) && (xArr[idx2] >= iLeftX || xArr[idxr] >= iLeftX) )
			{
				//use for interpolation
				float dzl = (zArr[idxl] - zArr[idx2]) * inv_dyl;
				float dzr = (zArr[idxr] - zArr[idx2]) * inv_dyr;

				float dul = (uArr[idxl] - uArr[idx2]) * inv_dyl;
				float dvl = (vArr[idxl] - vArr[idx2]) * inv_dyl;
				float dur = (uArr[idxr] - uArr[idx2]) * inv_dyr;
				float dvr = (vArr[idxr] - vArr[idx2]) * inv_dyr;

				float drl = (rArr[idxl] - rArr[idx2]) * inv_dyl;
				float dgl = (gArr[idxl] - gArr[idx2]) * inv_dyl;
				float dbl = (bArr[idxl] - bArr[idx2]) * inv_dyl;
				float dal = (aArr[idxl] - aArr[idx2]) * inv_dyl;
				float drr = (rArr[idxr] - rArr[idx2]) * inv_dyr;
				float dgr = (gArr[idxr] - gArr[idx2]) * inv_dyr;
				float dbr = (bArr[idxr] - bArr[idx2]) * inv_dyr;
				float dar = (aArr[idxr] - aArr[idx2]) * inv_dyr;

				float xl = xArr[idx2];
				float xr = xArr[idx2];
				float zl = zArr[idx2];
				float zr = zArr[idx2];
				float ul = uArr[idx2];
				float ur = uArr[idx2];
				float vl = vArr[idx2];
				float vr = vArr[idx2];
				float rl = rArr[idx2];
				float gl = gArr[idx2];
				float bl = bArr[idx2];
				float al = aArr[idx2];
				float rr = rArr[idx2];
				float gr = gArr[idx2];
				float br = bArr[idx2];
				float ar = aArr[idx2];

				dy = yt - iTopY;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					ul += dul * dy;
					ur += dur * dy;
					vl += dvl * dy;
					vr += dvr * dy;
					rl += drl * dy;
					rr += drr * dy;
					gl += dgl * dy;
					gr += dgr * dy;
					bl += dbl * dy;
					br += dbr * dy;
					al += dal * dy;
					ar += dar * dy;
					yt = iTopY;
				}
				ul *= (texWidth - 1);
				ur *= (texWidth - 1);
				vl *= (texHeight - 1);
				vr *= (texHeight - 1);
				dul *= (texWidth - 1);
				dur *= (texWidth - 1);
				dvl *= (texHeight - 1);
				dvr *= (texHeight - 1);

				int end_y = mid_y > iBottomY ? mid_y : iBottomY;
				while (yt >= end_y)
				{
					int dx = xr - xl;
					if(dx > 0){
						ui = ul;
						vi = vl;
						ri = rl;
						gi = gl;
						bi = bl;
						ai = al;
						zi = zl;

						float inv_dx = 1.0f / dx;
						float du = (ur - ul) * inv_dx;
						float dv = (vr - vl) * inv_dx;
						float dr = (rr - rl) * inv_dx;
						float dg = (gr - gl) * inv_dx;
						float db = (br - bl) * inv_dx;
						float da = (ar - al) * inv_dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							ri += dx * dr;
							gi += dx * dg;
							bi += dx * db;
							ai += dx * da;
							ui += dx * du;
							vi += dx * dv;
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yt * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yt * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{
							if(zi > *depthBuf){
								float inv_z = 1.0f / zi;
								float fu = ui * inv_z;
								float fv = vi * inv_z;
								int u = (int)(fu);
								int v = (int)(fv);
								float d0x = fu - u;
								float d0y = fv - v;
								float d1x = 1.0f - d0x;
								float d1y = 1.0f - d0y;
								byte *pImg = imgBuf + (v * texWidth + u) * 3;
								byte c0r = *pImg;
								byte c0g = *(++pImg);
								byte c0b = *(++pImg);
								byte c1r = *(++pImg);
								byte c1g = *(++pImg);
								byte c1b = *(++pImg);
								pImg += (texWidth * 3) - 5;
								byte c2r = *pImg;
								byte c2g = *(++pImg);
								byte c2b = *(++pImg);
								byte c3r = *(++pImg);
								byte c3g = *(++pImg);
								byte c3b = *(++pImg);

								float a = d1x * d1y;
								float b = d0x * d1y;
								float c = d1x * d0y;
								float d = d0x * d0y;

								*colorBuf = a * c0r + b * c1r + c * c2r + d * c3r;
								*(colorBuf + 1) = a * c0g + b * c1g + c * c2g + d * c3g;
								*(colorBuf + 2) = a * c0b + b * c1b + c * c2b + d * c3b;
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							ri += dr;
							gi += dg;
							bi += db;
							ai += da;
							ui += du;
							vi += dv;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					ul += dul;
					ur += dur;
					vl += dvl;
					vr += dvr;
					rl += drl;
					rr += drr;
					gl += dgl;
					gr += dgr;
					bl += dbl;
					br += dbr;
					al += dal;
					ar += dar;
					--yt;
				}
			}
		}
	}

	void SoftwarePipeline::drawTriangleSolidColorTex(const PolyTriangle &triangle, const Texture2D &tex)
	{
		const Vertex &v0 = triangle.tranList[0];
		const Vertex &v1 = triangle.tranList[1];
		const Vertex &v2 = triangle.tranList[2];

		drawTriangleSolidColorTex(v0, v1, v2, tex);
	}

	void SoftwarePipeline::drawTriangleSolidColorTex(const Vertex &v0, const Vertex & v1, const Vertex &v2, const Texture2D &tex)
	{
		int idx0 = 0;
		int idx1 = 1;
		int idx2 = 2;

		int xArr[3] = {(int)(v0.p.x + 0.5f), (int)(v1.p.x + 0.5f), (int)(v2.p.x + 0.5f)};
		int yArr[3] = {(int)(v0.p.y + 0.5f), (int)(v1.p.y + 0.5f), (int)(v2.p.y + 0.5f)};
		float zArr[3] = {v0.p.w, v1.p.w, v2.p.w};
		
		float uArr[3] = {v0.uv.x, v1.uv.x, v2.uv.x};
		float vArr[3] = {v0.uv.y, v1.uv.y, v2.uv.y};

		float rArr[3] = {v0.color.r * zArr[0], v1.color.r * zArr[1], v2.color.r * zArr[2]};
		float gArr[3] = {v0.color.g * zArr[0], v1.color.g * zArr[1], v2.color.g * zArr[2]};
		float bArr[3] = {v0.color.b * zArr[0], v1.color.b * zArr[1], v2.color.b * zArr[2]};
		float aArr[3] = {v0.color.a * zArr[0], v1.color.a * zArr[1], v2.color.a * zArr[2]};

		if((xArr[0] == xArr[1] && xArr[1] == xArr[2]) || (yArr[0] == yArr[1] && yArr[1] == yArr[2]))
		{
			return;
		}
		int tmp_idx = 0;
		//sort v0, v1, v2 by y value
		if(yArr[idx1] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx1;
			idx1 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx0]) {
			tmp_idx = idx0;
			idx0 = idx2;
			idx2 = tmp_idx;
		}
		if(yArr[idx2] < yArr[idx1]) {
			tmp_idx = idx1;
			idx1 = idx2;
			idx2 = tmp_idx;
		}//end sort

		if ( yArr[idx2] < iBottomY || yArr[idx0] > iTopY ||
    		(xArr[idx0] < iLeftX && xArr[idx1] < iLeftX && xArr[idx2] < iLeftX) ||
    		(xArr[idx0] > iRightX && xArr[idx1] > iRightX && xArr[idx2] > iRightX) )
		{
			return;
		}

		int bytesPerRow = iWidth * colorBytes;
		//tmp interpolation values
		float ui;
		float vi;
		float ri;
		float gi;
		float bi;
		float ai;
		float zi;

		int mid_y = yArr[idx1];
		int yb = yArr[idx0];
		int yt = yArr[idx2];

		int dy;

		byte *colorBuf;
		float *depthBuf;
		//calaculate the mipmap level
		int leftIdx = idx0;
		int rightIdx = idx0;
		leftIdx = xArr[leftIdx] > xArr[idx1] ? idx1 : leftIdx;
		leftIdx = xArr[leftIdx] > xArr[idx2] ? idx2 : leftIdx;
		rightIdx = xArr[rightIdx] < xArr[idx1] ? idx1 : rightIdx;
		rightIdx = xArr[rightIdx] < xArr[idx2] ? idx2 : rightIdx;
		float midLen = (xArr[leftIdx] - xArr[rightIdx]);
		float texMidLen = (uArr[leftIdx] / zArr[leftIdx] - uArr[rightIdx] / zArr[rightIdx]) * tex.getWidth();
		int mipmapLv = 0;
		if(midLen != 0)
			mipmapLv = int((texMidLen / midLen) + 0.5f);
		int maxMipmapLv = tex.getMaxMipmapLevel();
		mipmapLv = maxMipmapLv < mipmapLv ? maxMipmapLv : mipmapLv;
		mipmapLv = 0 > mipmapLv ? 0 : mipmapLv;
		Image *img = tex.getImage(mipmapLv);
		byte *imgBuf = img->getData();
		int texWidth = img->getWidth();
		int texHeight = img->getHeight();
		//draw down triangle
		if(yArr[idx0] != yArr[idx1] && mid_y >= iBottomY && yb <= iTopY) {
			//sort v1, v2 by dx value
			int idxl = idx1;
			int idxr = idx2;
			float inv_dyl = 1.0f / (yArr[idxl] - yArr[idx0]);
			float inv_dyr = 1.0f / (yArr[idxr] - yArr[idx0]);
			float dxl = (float)(xArr[idxl] - xArr[idx0]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx0]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx0] <= iRightX) && (xArr[idxr] >= iLeftX || xArr[idx0] >= iLeftX) )
			{
				float dzl = (zArr[idxl] - zArr[idx0]) * inv_dyl;
				float dzr = (zArr[idxr] - zArr[idx0]) * inv_dyr;

				float dul = (uArr[idxl] - uArr[idx0]) * inv_dyl;
				float dvl = (vArr[idxl] - vArr[idx0]) * inv_dyl;
				float dur = (uArr[idxr] - uArr[idx0]) * inv_dyr;
				float dvr = (vArr[idxr] - vArr[idx0]) * inv_dyr;

				float drl = (rArr[idxl] - rArr[idx0]) * inv_dyl;
				float dgl = (gArr[idxl] - gArr[idx0]) * inv_dyl;
				float dbl = (bArr[idxl] - bArr[idx0]) * inv_dyl;
				float dal = (aArr[idxl] - aArr[idx0]) * inv_dyl;
				float drr = (rArr[idxr] - rArr[idx0]) * inv_dyr;
				float dgr = (gArr[idxr] - gArr[idx0]) * inv_dyr;
				float dbr = (bArr[idxr] - bArr[idx0]) * inv_dyr;
				float dar = (aArr[idxr] - aArr[idx0]) * inv_dyr;

				float xl = xArr[idx0];
				float xr = xArr[idx0];
				float zl = zArr[idx0];
				float zr = zArr[idx0];
				float ul = uArr[idx0];
				float ur = uArr[idx0];
				float vl = vArr[idx0];
				float vr = vArr[idx0];
				float rl = rArr[idx0];
				float gl = gArr[idx0];
				float bl = bArr[idx0];
				float al = aArr[idx0];
				float rr = rArr[idx0];
				float gr = gArr[idx0];
				float br = bArr[idx0];
				float ar = aArr[idx0];

				dy = iBottomY - yb;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					ul += dul * dy;
					ur += dur * dy;
					vl += dvl * dy;
					vr += dvr * dy;
					rl += drl * dy;
					rr += drr * dy;
					gl += dgl * dy;
					gr += dgr * dy;
					bl += dbl * dy;
					br += dbr * dy;
					al += dal * dy;
					ar += dar * dy;
					yb = iBottomY;
				}
				ul *= (texWidth - 1);
				ur *= (texWidth - 1);
				vl *= (texHeight - 1);
				vr *= (texHeight - 1);
				dul *= (texWidth - 1);
				dur *= (texWidth - 1);
				dvl *= (texHeight - 1);
				dvr *= (texHeight - 1);

				int end_y = mid_y > iTopY ? iTopY : mid_y;
				while(yb <= end_y)
				{
					int dx = xr - xl;
					if(dx > 0){
						ui = ul;
						vi = vl;
						ri = rl;
						gi = gl;
						bi = bl;
						ai = al;
						zi = zl;

						float inv_dx = 1.0f / dx;
						float du = (ur - ul) * inv_dx;
						float dv = (vr - vl) * inv_dx;
						float dr = (rr - rl) * inv_dx;
						float dg = (gr - gl) * inv_dx;
						float db = (br - bl) * inv_dx;
						float da = (ar - al) * inv_dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							ri += dx * dr;
							gi += dx * dg;
							bi += dx * db;
							ai += dx * da;
							ui += dx * du;
							vi += dx * dv;
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yb * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yb * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{	
							if(zi > *depthBuf){
								float inv_z = 1.0f / zi;
								int u = (int)(ui * inv_z + 0.5f);
								int v = (int)(vi * inv_z + 0.5f);
								byte *pImg = imgBuf + (v * texWidth + u) * 3;
								byte tr = *pImg;
								byte tg = *(pImg + 1);
								byte tb = *(pImg + 2);
								*colorBuf = (byte)(ri * inv_z) * tr >> 8;
								*(colorBuf + 1) = (byte)(gi * inv_z) * tg >> 8;
								*(colorBuf + 2) = (byte)(bi * inv_z) * tb >> 8;
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							ri += dr;
							gi += dg;
							bi += db;
							ai += da;
							ui += du;
							vi += dv;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					ul += dul;
					ur += dur;
					vl += dvl;
					vr += dvr;
					rl += drl;
					rr += drr;
					gl += dgl;
					gr += dgr;
					bl += dbl;
					br += dbr;
					al += dal;
					ar += dar;
					++yb;
				}
			}
		}
		//draw top triangle
		if(yArr[idx2] != yArr[idx1] && mid_y <= iTopY && yt >= iBottomY) {
			//sort v0, v1 by dx value
			int idxl = idx0;
			int idxr = idx1;
			float inv_dyl = 1.0f / (yArr[idx2] - yArr[idxl]);
			float inv_dyr = 1.0f / (yArr[idx2] - yArr[idxr]);
			float dxl = (float)(xArr[idxl] - xArr[idx2]) * inv_dyl;
			float dxr = (float)(xArr[idxr] - xArr[idx2]) * inv_dyr;
			if( dxl > dxr )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
				float tmp_inv_dy = inv_dyl;
				inv_dyl = inv_dyr;
				inv_dyr = tmp_inv_dy;
				float tmp_dx = dxl;
				dxl = dxr;
				dxr = tmp_dx;
			}//end sort
			// if( (xArr[idxl] <= iRightX || xArr[idx2] <= iRightX) && (xArr[idx2] >= iLeftX || xArr[idxr] >= iLeftX) )
			{
				//use for interpolation
				float dzl = (zArr[idxl] - zArr[idx2]) * inv_dyl;
				float dzr = (zArr[idxr] - zArr[idx2]) * inv_dyr;

				float dul = (uArr[idxl] - uArr[idx2]) * inv_dyl;
				float dvl = (vArr[idxl] - vArr[idx2]) * inv_dyl;
				float dur = (uArr[idxr] - uArr[idx2]) * inv_dyr;
				float dvr = (vArr[idxr] - vArr[idx2]) * inv_dyr;

				float drl = (rArr[idxl] - rArr[idx2]) * inv_dyl;
				float dgl = (gArr[idxl] - gArr[idx2]) * inv_dyl;
				float dbl = (bArr[idxl] - bArr[idx2]) * inv_dyl;
				float dal = (aArr[idxl] - aArr[idx2]) * inv_dyl;
				float drr = (rArr[idxr] - rArr[idx2]) * inv_dyr;
				float dgr = (gArr[idxr] - gArr[idx2]) * inv_dyr;
				float dbr = (bArr[idxr] - bArr[idx2]) * inv_dyr;
				float dar = (aArr[idxr] - aArr[idx2]) * inv_dyr;

				float xl = xArr[idx2];
				float xr = xArr[idx2];
				float zl = zArr[idx2];
				float zr = zArr[idx2];
				float ul = uArr[idx2];
				float ur = uArr[idx2];
				float vl = vArr[idx2];
				float vr = vArr[idx2];
				float rl = rArr[idx2];
				float gl = gArr[idx2];
				float bl = bArr[idx2];
				float al = aArr[idx2];
				float rr = rArr[idx2];
				float gr = gArr[idx2];
				float br = bArr[idx2];
				float ar = aArr[idx2];

				dy = yt - iTopY;
				if(dy > 0)
				{
					xl += dxl * dy;
					xr += dxr * dy;
					zl += dzl * dy;
					zr += dzr * dy;
					ul += dul * dy;
					ur += dur * dy;
					vl += dvl * dy;
					vr += dvr * dy;
					rl += drl * dy;
					rr += drr * dy;
					gl += dgl * dy;
					gr += dgr * dy;
					bl += dbl * dy;
					br += dbr * dy;
					al += dal * dy;
					ar += dar * dy;
					yt = iTopY;
				}
				ul *= (texWidth - 1);
				ur *= (texWidth - 1);
				vl *= (texHeight - 1);
				vr *= (texHeight - 1);
				dul *= (texWidth - 1);
				dur *= (texWidth - 1);
				dvl *= (texHeight - 1);
				dvr *= (texHeight - 1);

				int end_y = mid_y > iBottomY ? mid_y : iBottomY;
				while (yt >= end_y)
				{
					int dx = xr - xl;
					if(dx > 0){
						ui = ul;
						vi = vl;
						ri = rl;
						gi = gl;
						bi = bl;
						ai = al;
						zi = zl;

						float inv_dx = 1.0f / dx;
						float du = (ur - ul) * inv_dx;
						float dv = (vr - vl) * inv_dx;
						float dr = (rr - rl) * inv_dx;
						float dg = (gr - gl) * inv_dx;
						float db = (br - bl) * inv_dx;
						float da = (ar - al) * inv_dx;
						float dz = (zr - zl) * inv_dx;

						int i_xl = Math::ceil(xl);
						dx = iLeftX - i_xl;
						if(dx > 0)
						{
							ri += dx * dr;
							gi += dx * dg;
							bi += dx * db;
							ai += dx * da;
							ui += dx * du;
							vi += dx * dv;
							zi += dx * dz;
							i_xl = iLeftX;
						}
						int i_xr = Math::ceil(xr);
						i_xr = i_xr < iRightX ? i_xr : iRightX;
						colorBuf = _bColorBuffer + yt * bytesPerRow + i_xl * colorBytes;
						depthBuf = _fDepthBuffer + yt * iWidth + i_xl;
						for (int x = i_xl; x < i_xr; ++x)
						{
							if(zi > *depthBuf){
								float inv_z = 1.0f / zi;
								int u = (int)(ui * inv_z + 0.5f);
								int v = (int)(vi * inv_z + 0.5f);
								byte *pImg = imgBuf + (v * texWidth + u) * 3;
								byte tr = *pImg;
								byte tg = *(pImg + 1);
								byte tb = *(pImg + 2);
								*colorBuf = (byte)(ri * inv_z) * tr >> 8;
								*(colorBuf + 1) = (byte)(gi * inv_z) * tg >> 8;
								*(colorBuf + 2) = (byte)(bi * inv_z) * tb >> 8;
								*depthBuf = zi;
							}
							colorBuf += colorBytes;
							++depthBuf;
							ri += dr;
							gi += dg;
							bi += db;
							ai += da;
							ui += du;
							vi += dv;
							zi += dz;
						}
					}
					xl += dxl;
					xr += dxr;
					zl += dzl;
					zr += dzr;
					ul += dul;
					ur += dur;
					vl += dvl;
					vr += dvr;
					rl += drl;
					rr += drr;
					gl += dgl;
					gr += dgr;
					bl += dbl;
					br += dbr;
					al += dal;
					ar += dar;
					--yt;
				}
			}
		}
	}
	
	void SoftwarePipeline::setClearColor(const ColorRGBA &color)
	{
		_clearColor.r = (byte)(255 * color.r);
		_clearColor.g = (byte)(255 * color.g);
		_clearColor.b = (byte)(255 * color.b);
		glClearColor(1,0,1,1);
	}

	void SoftwarePipeline::clearBuffer(ATTR_FLAG flags)
	{
		_clearColorBuffer();
		if(flags & ATTR_FLAG::DEPTH_BUFFER)
		{
			_clearDepthBuffer();
		}
	}

	void SoftwarePipeline::clearBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		_clearColorBuffer();
		_clearDepthBuffer();
	}

	void SoftwarePipeline::_clearColorBuffer()
	{
		int pixelCount = iHeight * iWidth;
		byte *pos = _bColorBuffer;
		unsigned int color = (255<<24) | (255<<16) | (255<<8) | 255;
		for(int idx = 0; idx < pixelCount; ++idx){
			*pos = _clearColor.r;
			*(pos + 1) =  _clearColor.g;
			*(pos + 2) =  _clearColor.b;
			pos +=  colorBytes;
		}
	}

	void SoftwarePipeline::_clearDepthBuffer()
	{
		if(_attr & ATTR_FLAG::DEPTH_BUFFER)
		{
			int count = iWidth * iHeight;
			for (int i = 0; i < count; ++i)
			{
				_fDepthBuffer[i] = 0.0f;
			}
		}
	}

	void SoftwarePipeline::swapBuffer()
	{
		_program.use();
		glUniformMatrix4fv(uniformMat, 1, false, mv.m);
		
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _screenTex);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, _bColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _bColorBuffer);
		glUniform1i(_texIdx, 0);
		glBindVertexArray(_screenRect);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
		//glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		//glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		//glWindowPos2d(0, 0);
		//glDrawPixels(iWidth,iHeight, GL_RGB, GL_UNSIGNED_BYTE, _bColorBuffer);
	}

	void SoftwarePipeline::resizeViewPort(int width, int height)
	{
		iWidth = width;
		iHeight = height;

		GP_SAFE_DELETE_ARRAY(_bColorBuffer);
		_bColorBuffer = GP_NEW_ARRAY(byte, iWidth * iHeight * colorBytes);

		if(_fDepthBuffer)
		{
			GP_DELETE_ARRAY(_fDepthBuffer);
			_fDepthBuffer = GP_NEW_ARRAY(float, iWidth * iHeight);	
		}
	}

	void SoftwarePipeline::drawArray(RenderList &buffer)
	{
		int faceCount = buffer.size();
		if(buffer.isEnabled(POLY_ATTR::ENABLE_TEX))
		{
			Texture2D *tex = buffer.getTexture();
			if(tex){
				if(buffer.isEnabled(POLY_ATTR::ENABLE_COLOR)){
					for(int index = 0; index < faceCount; ++index)
					{
						auto &polygen = buffer.triangleArray[index];
						if(!(polygen.attr & POLY_ATTR::CULLED))
						{
							drawTriangleSolidTexNearest(polygen, *tex);
							// drawTriangleSolidColorTex(polygen, *tex);
						}
					}
					return;
				}else{
					for(int index = 0; index < faceCount; ++index)
					{
						auto &polygen = buffer.triangleArray[index];
						if(!(polygen.attr & POLY_ATTR::CULLED))
						{
							drawTriangleSolidTexLinear(polygen, *tex);
						}
					}
					return;
				}	
			}else{
				if(buffer.isEnabled(POLY_ATTR::ENABLE_COLOR)){
					for(int index = 0; index < faceCount; ++index)
					{
						auto &polygen = buffer.triangleArray[index];
						if(!(polygen.attr & POLY_ATTR::CULLED))
						{
							drawTriangleSolidColor(polygen);
						}
					}
					return;
				}
			}
		}
		for(int index = 0; index < faceCount; ++index)
		{
			auto &polygen = buffer.triangleArray[index];
			if(!(polygen.attr & POLY_ATTR::CULLED))
			{
				drawTriangleSolidFlat(buffer.triangleArray[index]);
			}
		}
	}
	
	void SoftwarePipeline::drawElements(PolyObject &buffer, uint_32 offset,  uint_32 faceCount)
	{
		if(buffer.isEnabled(POLY_ATTR::ENABLE_TEX))
		{
			Texture2D *tex = buffer.getTexture();
			if(tex){
				if(buffer.isEnabled(POLY_ATTR::ENABLE_COLOR)){
					for(int index = offset; index < faceCount; ++index)
					{
						auto polygen = buffer.polygens[index];
						if(!(polygen.attr & POLY_ATTR::CULLED))
						{
							Vertex &v0 = buffer.tranList[polygen.indices[0]];
							Vertex &v1 = buffer.tranList[polygen.indices[1]];
							Vertex &v2 = buffer.tranList[polygen.indices[2]];
							// drawTriangleSolidTexNearest(v0, v1, v2, *tex);
							drawTriangleSolidColorTex(v0, v1, v2, *tex);
						}
					}	
					return;
				}else{
					for(int index = offset; index < faceCount; ++index)
					{
						auto polygen = buffer.polygens[index];
						if(!(polygen.attr & POLY_ATTR::CULLED))
						{
							Vertex &v0 = buffer.tranList[polygen.indices[0]];
							Vertex &v1 = buffer.tranList[polygen.indices[1]];
							Vertex &v2 = buffer.tranList[polygen.indices[2]];
							drawTriangleSolidTexLinear(v0, v1, v2, *tex);
						}
					}
					return;
				}
			}
		}else{
			for(int index = offset; index < faceCount; ++index)
			{
				auto polygen = buffer.polygens[index];
				if(!(polygen.attr & POLY_ATTR::CULLED))
				{
					Vertex &v0 = buffer.tranList[polygen.indices[0]];
					Vertex &v1 = buffer.tranList[polygen.indices[1]];
					Vertex &v2 = buffer.tranList[polygen.indices[2]];
					drawTriangleSolidColor(v0, v1, v2);
				}
			}
			return;
		}
		for(int index = offset; index < faceCount; ++index)
		{
			auto polygen = buffer.polygens[index];
			if(!(polygen.attr & POLY_ATTR::CULLED))
			{
				Vertex &v0 = buffer.tranList[polygen.indices[0]];
				Vertex &v1 = buffer.tranList[polygen.indices[1]];
				Vertex &v2 = buffer.tranList[polygen.indices[2]];
				drawTriangleSolidFlat(v0, v1, v2);
			}
		}
	}

	void SoftwarePipeline::removeBackFaces(RenderList &buffer, uint_32 offset, uint_32 faceCount)
	{
		for (int index = offset; index < offset + faceCount; ++index)
		{
			buffer.triangleArray[index].attr = 0;
			buffer.triangleArray[index].tranList[0] = buffer.triangleArray[index].localList[0];
			buffer.triangleArray[index].tranList[1] = buffer.triangleArray[index].localList[1];
			buffer.triangleArray[index].tranList[2] = buffer.triangleArray[index].localList[2];
			vec3f p0 = buffer.triangleArray[index].tranList[0].p;
			vec3f p1 = buffer.triangleArray[index].tranList[1].p;
			vec3f p2 = buffer.triangleArray[index].tranList[2].p;
			vec3f v0(p0, p1);
			vec3f v1(p1, p2);
			vec3f normal = v0.crossMul(v1);
			buffer.triangleArray[index].normal = normal;
			float dot = p0.dotMul(normal);
			if(dot * _cullMode >= 0)
			{
				setBit(buffer.triangleArray[index].attr, POLY_ATTR::CULLED);
			}
		}
	}

	void SoftwarePipeline::removeBackFaces(PolyObject &buffer, uint_32 offset, uint_32 faceCount)
	{
		for (int index = offset; index < offset + faceCount; ++index)
		{
			auto &polygen = buffer.polygens[index];
			if(polygen.attr &= POLY_ATTR::CULLED)
			polygen.attr &= RESET_VISIBLE_FLAG;
			buffer.tranList = buffer.localList;
			vec3f p0 = buffer.tranList[polygen.indices[0]].p;
			vec3f p1 = buffer.tranList[polygen.indices[1]].p;
			vec3f p2 = buffer.tranList[polygen.indices[2]].p;
			vec3f v0(p0, p1);
			vec3f v1(p1, p2);
			vec3f normal = v0.crossMul(v1);
			polygen.normal = normal;
			float dot = p0.dotMul(normal);
			if(dot * _cullMode >= 0)
			{
				setBit(polygen.attr, POLY_ATTR::CULLED);
			}
		}
	}

	void SoftwarePipeline::insertObjectToRenderList(RenderList &list, PolyObject &obj, uint_32 offset, uint_32 faceCount)
	{
		for (int index = offset; index < offset + faceCount; ++index)
		{
			auto &polygen = obj.polygens[index];
			if( !(polygen.attr & POLY_ATTR::CULLED) )
			{
				list.insertTrianglePolygen(obj.tranList[polygen.indices[0]], obj.tranList[polygen.indices[1]], obj.tranList[polygen.indices[2]],
					polygen.normal, polygen.attr, polygen.normalLen);
			}
		}
		list.setTexture(obj.getTexture());
		list.setAttrCode(obj.getAttrCode());
	}

	void SoftwarePipeline::CVVClip(PolyObject &buffer, uint_32 offset, uint_32 faceCount)	
	{
		for (int index = offset; index < offset + faceCount; ++index)
		{
			auto &polygen = buffer.polygens[index];
			if( !(polygen.attr & POLY_ATTR::CULLED) )
			{
				vec4f &p0 = buffer.tranList[polygen.indices[0]].p;
				vec4f &p1 = buffer.tranList[polygen.indices[1]].p;
				vec4f &p2 = buffer.tranList[polygen.indices[2]].p;
				if( (p0.z < -p0.w && p1.z < -p1.w && p2.z < -p2.w) || (p0.z > p0.w && p1.z > p1.w && p2.z > p2.w) )
				{
					setBit(polygen.attr, POLY_ATTR::CULLED);
				}
			}
		}
	}

	void SoftwarePipeline::nearFarPlaneClip(RenderList &buffer, float nearZ, float farZ)
	{
		int faceCount = buffer.size();
		for (int index = 0; index < faceCount; ++index)
		{
			auto &triangle = buffer.triangleArray[index];
			if( !(triangle.attr & POLY_ATTR::CULLED) )
			{
				vec4f &p0 = triangle.tranList[0].p;
				vec4f &p1 = triangle.tranList[1].p;
				vec4f &p2 = triangle.tranList[2].p;
				int clipStatus[3];
				int vertices_in = 0;
				if(p0.z > -nearZ)
				{
					clipStatus[0] |= CLIP_NZ;
				}else if(p0.z < -farZ)
				{
					clipStatus[0] |= CLIP_FZ;
				}else{
					clipStatus[0] |= CLIP_IZ;
					++vertices_in;
				}
				if(p1.z > -nearZ)
				{
					clipStatus[1] |= CLIP_NZ;
				}else if(p1.z < -farZ)
				{
					clipStatus[1] |= CLIP_FZ;
				}else{
					clipStatus[1] |= CLIP_IZ;
					++vertices_in;
				}
				if(p2.z > -nearZ)
				{
					clipStatus[2] |= CLIP_NZ;
				}else if(p2.z < -farZ)
				{
					clipStatus[2] |= CLIP_FZ;
				}else{
					clipStatus[2] |= CLIP_IZ;
					++vertices_in;
				}

				int idx0, idx1, idx2;
				//triangle totally out of the range at z axis
				if(vertices_in == 3)
				{
					continue;
				}else if(vertices_in == 0)
				{
					setBit(triangle.attr, POLY_ATTR::CULLED);
				}else if(vertices_in == 1)
				{
					//idx1, idx2 will be changed
					if(clipStatus[0] & CLIP_IZ)
					{
						idx0 = 0;
						idx1 = 1;
						idx2 = 2;
					}
					else if(clipStatus[1] & CLIP_IZ)
					{
						idx0 = 1;
						idx1 = 2;
						idx2 = 0;
					}
					else
					{
						idx0 = 2;
						idx1 = 0;
						idx2 = 1;
					}
					vec4f &v0 = triangle.tranList[idx0].p;
					vec4f &v1 = triangle.tranList[idx1].p;
					vec4f &v2 = triangle.tranList[idx2].p;
					float t10 = v0.z / (v0.z - v1.z);
					
				}
				else if(vertices_in == 2)
				{
					//idx0 will be changed
					if(clipStatus[0] & CLIP_NZ)
					{
						idx0 = 0;
						idx1 = 1;
						idx2 = 2;
					}
					else if(clipStatus[1] & CLIP_NZ)
					{
						idx0 = 1;
						idx1 = 2;
						idx2 = 0;
					}
					else
					{
						idx0 = 2;
						idx1 = 0;
						idx2 = 1;
					}
				}
			}
		}
	}

	void SoftwarePipeline::CVVClip(RenderList &buffer)
	{
		int faceCount = buffer.size();
		for (int index = 0; index < faceCount; ++index)
		{
			auto &triangle = buffer.triangleArray[index];
			if( !(triangle.attr & POLY_ATTR::CULLED) )
			{
				int clipStatus[] = {0, 0, 0};
				Vertex &tv0 = triangle.tranList[0];
				Vertex &tv1 = triangle.tranList[1];
				Vertex &tv2 = triangle.tranList[2];
				vec4f &p0 = tv0.p;
				vec4f &p1 = tv1.p;
				vec4f &p2 = tv2.p;
				float invWArr[] = {1.0f/p0.w, 1.0f/p1.w, 1.0f/p2.w};
				p0 *= invWArr[0];
				p1 *= invWArr[1];
				p2 *= invWArr[2];
				vec2f &uv0 = tv0.uv;
				uv0.x *= invWArr[0];
				uv0.y *= invWArr[0];
				vec2f &uv1 = tv1.uv;
				uv1.x *= invWArr[1];
				uv1.y *= invWArr[1];
				vec2f &uv2 = tv2.uv;
				uv2.x *= invWArr[2];
				uv2.y *= invWArr[2];

				int vertices_in = 0;
				int vertices_front = 0;
				if(p0.w < 0 || p0.z < -1)
				{
					clipStatus[0] |= CLIP_NZ;
					++vertices_front;
				}else if(p0.z > 1)
				{
					clipStatus[0] |= CLIP_FZ;
				}else{
					clipStatus[0] |= CLIP_IZ;
					++vertices_in;
				}
				if(p1.w < 0 ||p1.z < -1)
				{
					clipStatus[1] |= CLIP_NZ;
					++vertices_front;
				}else if(p1.z > 1)
				{
					clipStatus[1] |= CLIP_FZ;
				}else{
					clipStatus[1] |= CLIP_IZ;
					++vertices_in;
				}
				if(p2.w < 0 ||p2.z < -1)
				{
					clipStatus[2] |= CLIP_NZ;
					++vertices_front;
				}else if(p2.z > 1)
				{
					clipStatus[2] |= CLIP_FZ;
				}else{
					clipStatus[2] |= CLIP_IZ;
					++vertices_in;
				}

				p0.w = invWArr[0];
				p1.w = invWArr[1];
				p2.w = invWArr[2];

				int idx0, idx1, idx2;
				//triangle totally out of the range at z axis
				if( vertices_in == 0 )
				{
					setBit(triangle.attr, POLY_ATTR::CULLED);
				}
				else if(vertices_front == 2){
					//idx1, idx2 will be changed
					if(clipStatus[0] & CLIP_IZ)
					{
						idx0 = 0;
						idx1 = 1;
						idx2 = 2;
					}else if(clipStatus[1] & CLIP_IZ)
					{
						idx0 = 1;
						idx1 = 2;
						idx2 = 0;
					}else{
						idx0 = 2;
						idx1 = 0;
						idx2 = 1;
					}

					Vertex &v0 = triangle.tranList[idx0];
					Vertex &v1 = triangle.tranList[idx1];
					Vertex &v2 = triangle.tranList[idx2];

					float xArr[3] = {v0.p.x, v1.p.x, v2.p.x};
					float yArr[3] = {v0.p.y, v1.p.y, v2.p.y};
					float zArr[3] = {v0.p.z, v1.p.z, v2.p.z};
					float wArr[3] = {v0.p.w, v1.p.w, v2.p.w};
					float rArr[3] = {v0.color.r * wArr[0], v1.color.r * wArr[1], v2.color.r * wArr[2]};
					float gArr[3] = {v0.color.g * wArr[0], v1.color.g * wArr[1], v2.color.g * wArr[2]};
					float bArr[3] = {v0.color.b * wArr[0], v1.color.b * wArr[1], v2.color.b * wArr[2]};
					float aArr[3] = {v0.color.a * wArr[0], v1.color.a * wArr[1], v2.color.a * wArr[2]};
					float uArr[3] = {v0.uv.x, v1.uv.x, v2.uv.x};
					float vArr[3] = {v0.uv.y, v1.uv.y, v2.uv.y};

					float ZDelta01 = (-1 - zArr[0]) / (zArr[1] - zArr[0]);
					float ZDelta02 = (-1 - zArr[0]) / (zArr[2] - zArr[0]);
					float x01 = xArr[0] + (xArr[1] - xArr[0]) * ZDelta01;
					float x02 = xArr[0] + (xArr[2] - xArr[0]) * ZDelta02;
					float y01 = yArr[0] + (yArr[1] - yArr[0]) * ZDelta01;
					float y02 = yArr[0] + (yArr[2] - yArr[0]) * ZDelta02;
					float z01 = -1;
					float z02 = -1;
					float w01 = wArr[0] + (wArr[1] - wArr[0]) * ZDelta01;
					float w02 = wArr[0] + (wArr[2] - wArr[0]) * ZDelta02;
					float u01 = uArr[0] + (uArr[1] - uArr[0]) * ZDelta01;
					float u02 = uArr[0] + (uArr[2] - uArr[0]) * ZDelta02;
					float v01 = vArr[0] + (vArr[1] - vArr[0]) * ZDelta01;
					float v02 = vArr[0] + (vArr[2] - vArr[0]) * ZDelta02;
					float r01 = rArr[0] + (rArr[1] - rArr[0]) * ZDelta01;
					float r02 = rArr[0] + (rArr[2] - rArr[0]) * ZDelta02;
					float g01 = gArr[0] + (gArr[1] - gArr[0]) * ZDelta01;
					float g02 = gArr[0] + (gArr[2] - gArr[0]) * ZDelta02;
					float b01 = bArr[0] + (bArr[1] - bArr[0]) * ZDelta01;
					float b02 = bArr[0] + (bArr[2] - bArr[0]) * ZDelta02;
					float a01 = aArr[0] + (aArr[1] - aArr[0]) * ZDelta01;
					float a02 = aArr[0] + (aArr[2] - aArr[0]) * ZDelta02;
					v1.p.w = w01;
					v1.p.x = x01;
					v1.p.y = y01;
					v1.p.z = z01;
					v1.uv.x = u01;
					v1.uv.y = v01;
					float invZ01 = 1.0f / w01;
					v1.color.r = r01 * invZ01;
					v1.color.g = g01 * invZ01;
					v1.color.b = b01 * invZ01;
					v1.color.a = a01 * invZ01;
					v2.p.w = w02;
					v2.p.x = x02;
					v2.p.y = y02;
					v2.p.z = z02;
					v2.uv.x = u02;
					v2.uv.y = v02;
					float invZ02 = 1.0f / w02;
					v2.color.r = r02 * invZ02;
					v2.color.g = g02 * invZ02;
					v2.color.b = b02 * invZ02;
					v2.color.a = a02 * invZ02;

				}else if(vertices_front == 1){
					//idx0 will be changed
					if(clipStatus[0] & CLIP_NZ)
					{
						idx0 = 0;
						idx1 = 1;
						idx2 = 2;
					}else if(clipStatus[1] & CLIP_NZ)
					{
						idx0 = 1;
						idx1 = 2;
						idx2 = 0;
					}else{
						idx0 = 2;
						idx1 = 0;
						idx2 = 1;
					}
					Vertex &v0 = triangle.tranList[idx0];
					Vertex &v1 = triangle.tranList[idx1];
					Vertex &v2 = triangle.tranList[idx2];
					float xArr[3] = {v0.p.x, v1.p.x, v2.p.x};
					float yArr[3] = {v0.p.y, v1.p.y, v2.p.y};
					float zArr[3] = {v0.p.z, v1.p.z, v2.p.z};
					float wArr[3] = {v0.p.w, v1.p.w, v2.p.w};
					float rArr[3] = {v0.color.r * wArr[0], v1.color.r * wArr[1], v2.color.r * wArr[2]};
					float gArr[3] = {v0.color.g * wArr[0], v1.color.g * wArr[1], v2.color.g * wArr[2]};
					float bArr[3] = {v0.color.b * wArr[0], v1.color.b * wArr[1], v2.color.b * wArr[2]};
					float aArr[3] = {v0.color.a * wArr[0], v1.color.a * wArr[1], v2.color.a * wArr[2]};
					float uArr[3] = {v0.uv.x, v1.uv.x, v2.uv.x};
					float vArr[3] = {v0.uv.y, v1.uv.y, v2.uv.y};

					float ZDelta01 = (-1 - zArr[0]) / (zArr[1] - zArr[0]);
					float ZDelta02 = (-1 - zArr[0]) / (zArr[2] - zArr[0]);
					float x01 = xArr[0] + (xArr[1] - xArr[0]) * ZDelta01;
					float x02 = xArr[0] + (xArr[2] - xArr[0]) * ZDelta02;
					float y01 = yArr[0] + (yArr[1] - yArr[0]) * ZDelta01;
					float y02 = yArr[0] + (yArr[2] - yArr[0]) * ZDelta02;
					float z01 = -1;
					float z02 = -1;
					float w01 = wArr[0] + (wArr[1] - wArr[0]) * ZDelta01;
					float w02 = wArr[0] + (wArr[2] - wArr[0]) * ZDelta02;
					float u01 = uArr[0] + (uArr[1] - uArr[0]) * ZDelta01;
					float u02 = uArr[0] + (uArr[2] - uArr[0]) * ZDelta02;
					float v01 = vArr[0] + (vArr[1] - vArr[0]) * ZDelta01;
					float v02 = vArr[0] + (vArr[2] - vArr[0]) * ZDelta02;
					float r01 = rArr[0] + (rArr[1] - rArr[0]) * ZDelta01;
					float r02 = rArr[0] + (rArr[2] - rArr[0]) * ZDelta02;
					float g01 = gArr[0] + (gArr[1] - gArr[0]) * ZDelta01;
					float g02 = gArr[0] + (gArr[2] - gArr[0]) * ZDelta02;
					float b01 = bArr[0] + (bArr[1] - bArr[0]) * ZDelta01;
					float b02 = bArr[0] + (bArr[2] - bArr[0]) * ZDelta02;
					float a01 = aArr[0] + (aArr[1] - aArr[0]) * ZDelta01;
					float a02 = aArr[0] + (aArr[2] - aArr[0]) * ZDelta02;
					v0.p.x = x01;
					v0.p.y = y01;
					v0.p.z = z01;
					v0.p.w = w01;
					v0.uv.x = u01;
					v0.uv.y = v01;
					float invZ01 = 1.0f / w01;
					v0.color.r = r01 * invZ01;
					v0.color.g = g01 * invZ01;
					v0.color.b = b01 * invZ01;
					v0.color.a = a01 * invZ01;

					Vertex iv0;
					iv0.p.x = x02;
					iv0.p.y = y02;
					iv0.p.z = z02;
					iv0.p.w = w02;
					iv0.uv.x = u02; 
					iv0.uv.y = v02; 
					float invZ02 = 1.0f / w02;
					iv0.color.r = r02 * invZ02;
					iv0.color.g = g02 * invZ02;
					iv0.color.b = b02 * invZ02;
					iv0.color.a = a02 * invZ02;
					buffer.insertTrianglePolygen(iv0, v0, v2, triangle.normal, triangle.attr, triangle.normalLen);
				}
			}
		}
	}
	/*
	after this transform, the x param of each vertex will be in range[0, ScreenWidth - 1],
	the y param will be in range[0, ScreenHeight - 1],
	the z param will be in range [0, 1]
	and w param will be -1 / z
	*/
	void SoftwarePipeline::projectionToScreenTransform(RenderList &buffer)
	{
		int faceCount = buffer.size();
		for (int index = 0; index < faceCount; ++index)
		{
			buffer.triangleArray[index].tranList[0].p.x = (buffer.triangleArray[index].tranList[0].p.x + 1.0f) * iWidth * 0.5f;
			buffer.triangleArray[index].tranList[0].p.y = (buffer.triangleArray[index].tranList[0].p.y + 1.0f) * iHeight * 0.5f;
			buffer.triangleArray[index].tranList[0].p.z = (buffer.triangleArray[index].tranList[0].p.z + 1.0f) * 0.5f;

			buffer.triangleArray[index].tranList[1].p.x = (buffer.triangleArray[index].tranList[1].p.x + 1.0f) * iWidth * 0.5f;
			buffer.triangleArray[index].tranList[1].p.y = (buffer.triangleArray[index].tranList[1].p.y + 1.0f) * iHeight * 0.5f;
			buffer.triangleArray[index].tranList[1].p.z = (buffer.triangleArray[index].tranList[1].p.z + 1.0f) * 0.5f;

			buffer.triangleArray[index].tranList[2].p.x = (buffer.triangleArray[index].tranList[2].p.x + 1.0f) * iWidth * 0.5f;
			buffer.triangleArray[index].tranList[2].p.y = (buffer.triangleArray[index].tranList[2].p.y + 1.0f) * iHeight * 0.5f;
			buffer.triangleArray[index].tranList[2].p.z = (buffer.triangleArray[index].tranList[2].p.z + 1.0f) * 0.5f;
		}
	}
	//the w has been inversed at CVVClip
	void SoftwarePipeline::projectionToScreenTransform(PolyObject &buffer)
	{
		uint_32 size = buffer.vertexCount();
		for(int idx = 0; idx < size; ++idx)
		{
			vec4f &v = buffer.tranList[idx].p;
			v.x = (v.x + 1.0f) * 0.5f * iWidth;
			v.y = (v.y + 1.0f) * 0.5f * iHeight;
			v.z = (v.z + 1.0f) * 0.5f;
		}
	}

	void SoftwarePipeline::enable(ATTR_FLAG flag)
	{
		if(flag & ATTR_FLAG::DEPTH_BUFFER && !_fDepthBuffer)
		{
			_fDepthBuffer = GP_NEW_ARRAY(float, iWidth * iHeight);
		}

		_attr |= flag;
	}

	void SoftwarePipeline::disable(ATTR_FLAG flag)
	{
		_attr &= ~flag;
	}
}