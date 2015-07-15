#include "GPSoftwareRenderer.h"
#include "gp.h"
#include "gpmath.h"

#include <stdio.h>

namespace GPEngine3D{
	SoftwareRenderer::SoftwareRenderer(PixelFormat pixel_format, char depthBits, char stencilBits, bool doubleBuffer, int width, int height):
		Renderer(pFormat, cDepthBits, cStencilBits, doubleBuffer, width, height),
		_fDepthBuffer(nullptr),
		_attr(ATTR_FLAG::COLOR_BUFFER)
	{
		_bColorBuffer = GP_NEW_ARRAY(byte, iWidth * iHeight * colorBytes);
		_clearColor.r = 0;
		_clearColor.g = 0;
		_clearColor.b = 0;

		iBottomY = 0;
		iLeftX = 0;
		iTopY = iBottomY + iHeight - 1;
		iRightX = iLeftX + iWidth - 1;
	}

	SoftwareRenderer::~SoftwareRenderer()
	{
		GP_SAFE_DELETE_ARRAY(_bColorBuffer);
		GP_SAFE_DELETE_ARRAY(_fDepthBuffer);
	}

	bool SoftwareRenderer::initContent()
	{
		if(_bColorBuffer)
		{
			return true;
		}else{
			return false;
		}
	}

	void SoftwareRenderer::drawPoint(const Point2i &v, const Color3b &color)
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

	void SoftwareRenderer::drawPoint(int x, int y, const Color3b &color)
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

	void SoftwareRenderer::drawLine(int x0, int y0, int x1, int y1, const Color3b &color)
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

	void SoftwareRenderer::drawLine(const Point2i &v0, const Point2i &v1, const Color3b &color)
	{
		drawLine(v0.x, v0.y, v1.x, v1.y, color);
	}

	void SoftwareRenderer::drawTriangleSolid(const PolyTriangle &triangle)
	{
		const Vertex &v0 = triangle.tranList[0];
		const Vertex &v1 = triangle.tranList[1];
		const Vertex &v2 = triangle.tranList[2];

		drawTriangleSolid(v0, v1, v2);
	}

	void SoftwareRenderer::drawTriangleSolid(const Vertex &v0, const Vertex & v1, const Vertex &v2)
	{
		int idx0 = 0;
		int idx1 = 1;
		int idx2 = 2;

		int xArr[3] = {(int)(v0.p.x + 0.5f), (int)(v1.p.x + 0.5f), (int)(v2.p.x + 0.5f)};
		int yArr[3] = {(int)(v0.p.y + 0.5f), (int)(v1.p.y + 0.5f), (int)(v2.p.y + 0.5f)};
		float invZArr[3] = {v0.p.w, v1.p.w, v2.p.w};
		float uArr[3] = {v0.uv.x * v0.p.w, v1.uv.x * v1.p.w, v2.uv.x * v2.p.w};
		float vArr[3] = {v0.uv.y * v0.p.w, v1.uv.y * v1.p.w, v2.uv.y * v2.p.w};

		float rArr[3] = {v0.color.r * v0.p.w, v1.color.r * v1.p.w, v2.color.r * v2.p.w};
		float gArr[3] = {v0.color.g * v0.p.w, v1.color.g * v1.p.w, v2.color.g * v2.p.w};
		float bArr[3] = {v0.color.b * v0.p.w, v1.color.b * v1.p.w, v2.color.b * v2.p.w};
		float aArr[3] = {v0.color.a * v0.p.w, v1.color.a * v1.p.w, v2.color.a * v2.p.w};

		if((xArr[0] == xArr[1] && xArr[1] == xArr[2]) || (yArr[0] == yArr[1] && yArr[1] == yArr[2]))
		{
			return;
		}
		float tmp_idx = 0;
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
			//sort v1, v2 by x value
			int idxl = idx1;
			int idxr = idx2;
			int distX = xArr[idxl] - xArr[idxr];
			if( (distX > 0) || (distX == 0 && xArr[idx0] < xArr[idx1]) )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
			}//end sort
			if( (xArr[idxl] <= iRightX || xArr[idx0] <= iRightX) && (xArr[idxr] >= iLeftX || xArr[idx0] >= iLeftX) )
			{
				float inv_dyl = 1.0f / (yArr[idxl] - yArr[idx0]);
				float inv_dyr = 1.0f / (yArr[idxr] - yArr[idx0]);

				//use for interpolation
				float dzl = (invZArr[idxl] - invZArr[idx0]) * inv_dyl;
				float dxl = (float)(xArr[idxl] - xArr[idx0]) * inv_dyl;
				float dzr = (invZArr[idxr] - invZArr[idx0]) * inv_dyr;
				float dxr = (float)(xArr[idxr] - xArr[idx0]) * inv_dyr;

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
				float zl = invZArr[idx0];
				float zr = invZArr[idx0];
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
							if(*depthBuf > zi){
								*colorBuf = (byte)(ri / zi);
								*(colorBuf + 1) = (byte)(gi / zi);
								*(colorBuf + 2) = (byte)(bi / zi);
								colorBuf += colorBytes;
								*depthBuf = zi;
								++depthBuf;
							}

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
			//sort v0, v1 by x value
			int idxl = idx0;
			int idxr = idx1;
			int distX = xArr[idxl] - xArr[idxr];
			if( (distX > 0) || (distX == 0 && xArr[idx0] < xArr[idx2]) )
			{
				tmp_idx = idxl;
				idxl = idxr;
				idxr = tmp_idx;
			}//end sort
			if( (xArr[idxl] <= iRightX || xArr[idxr] <= iRightX) && (xArr[idx1] >= iLeftX || xArr[idxr] >= iLeftX) )
			{
				float inv_dyl = 1.0f / (yArr[idx2] - yArr[idxl]);
				float inv_dyr = 1.0f / (yArr[idx2] - yArr[idxr]);
				//use for interpolation
				float dzl = (invZArr[idxl] - invZArr[idx2]) * inv_dyl;
				float dzr = (invZArr[idxr] - invZArr[idx2]) * inv_dyr;
				float dxl = (float)(xArr[idxl] - xArr[idx2]) * inv_dyl;
				float dxr = (float)(xArr[idxr] - xArr[idx2]) * inv_dyr;

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
				float zl = invZArr[idx2];
				float zr = invZArr[idx2];
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
				while (yt > end_y)
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
							if(*depthBuf > zi){
								*colorBuf = (byte)(ri / zi);
								*(colorBuf + 1) = (byte)(gi / zi);
								*(colorBuf + 2) = (byte)(bi / zi);
								colorBuf += colorBytes;
								*depthBuf = zi;
								++depthBuf;
							}
							
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
	
	void SoftwareRenderer::drawTriangleSolid(int x0, int y0, int x1, int y1, int x2, int y2, const Color3b &color)
	{
		if((x0 == x1 && x1 == x2) || (y0 == y1 && y1 == y2))
		{
			return;
		}
		int tmp_x = 0;
		int tmp_y = 0;
		//sort v0, v1, v2 by y value
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
			tmp_x = x2;
			tmp_y = y2;
			x2 = x1;
			y2 = y1;
			x1 = tmp_x;
			y1 = tmp_y;
		}//end sort

		if ( y2 < iBottomY || y0 > iTopY ||
    		(x0 < iLeftX && x1 < iLeftX && x2 < iLeftX) ||
    		(x0 > iRightX && x1 > iRightX && x2 > iRightX) )
		{
			return;
		}

		int mid_y = y1;

		byte *buf;
		int bytesPerRow = iWidth * colorBytes;
		//draw down triangle
		if(y0 != y1) {
			//sort v1, v2 by x value
			if(x1 > x2)
			{
				tmp_x = x2;
				tmp_y = y2;
				x2 = x1;
				y2 = y1;
				x1 = tmp_x;
				y1 = tmp_y;
			}//end sort
			if((x1 <= iRightX || x0 <= iRightX) && (x2 >= iLeftX || x0 >= iLeftX))
			{
				float dx01 = (float)(x1 - x0) / (y1 - y0);
				float dx02 = (float)(x2 - x0) / (y2 - y0);
				float start_x = x0;
				float end_x = x0;
				int start_y = y0;
				while(start_y <= mid_y)
				{
					start_y = start_y > iBottomY ? start_y : iBottomY;
					int iStartX = Math::ceil(start_x);
					iStartX = iStartX > iLeftX ? iStartX : iLeftX;
					int iEndX = Math::ceil(end_x);
					iEndX = iEndX < iRightX ? iEndX : iRightX;
					buf = _bColorBuffer + start_y * bytesPerRow + iStartX * colorBytes;
					for (int x = iStartX; x < iEndX; ++x)
					{
						*buf = color.r;
						*(buf + 1) = color.g;
						*(buf + 2) = color.b;
						buf += colorBytes;
					}
					start_x += dx01;
					end_x += dx02;
					++start_y;
				}
			}
		}
		//draw top triangle
		if(y2 != y1) {
			//sort v0, v1 by x value
			if(x0 > x1)
			{
				tmp_x = x0;
				tmp_y = y0;
				x0 = x1;
				y0 = y1;
				x1 = tmp_x;
				y1 = tmp_y;
			}//end sort
			if((x0 <= iRightX || x2 <= iRightX) && (x1 >= iLeftX || x2 >= iLeftX))
			{
				float dx02 = (float)(x2 - x0) / (y2 - y0);
				float dx12 = (float)(x2 - x1) / (y2 - y1);
				int start_y = y2;
				start_y = start_y < iTopY ? start_y : iTopY;
				
				float start_x = x2;
				float end_x = x2;
				while (start_y > mid_y)
				{
					int iStartX = Math::ceil(start_x);
					iStartX = iStartX > iLeftX ? iStartX : iLeftX;
					int iEndX = Math::ceil(end_x);
					iEndX = iEndX < iRightX ? iEndX : iRightX;
					buf = _bColorBuffer + start_y * bytesPerRow + iStartX * colorBytes;
					for (int x = iStartX; x < iEndX; ++x)
					{
						*buf = color.r;
						*(buf + 1) = color.g;
						*(buf + 2) = color.b;
						buf += colorBytes;
					}
					start_x -= dx02;
					end_x -= dx12;
					--start_y;
				}				
			}
		}
	}
	
	void SoftwareRenderer::drawTriangleSolid(const Point2f &v0, const Point2f &v1, const Point2f &v2, const Color3b &color)
	{
		drawTriangleSolid(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, color);
	}
	
	void SoftwareRenderer::setClearColor(const ColorRGBA &color)
	{
		_clearColor.r = (byte)(255 * color.r);
		_clearColor.g = (byte)(255 * color.g);
		_clearColor.b = (byte)(255 * color.b);
	}

	void SoftwareRenderer::clearBuffer(ATTR_FLAG flags)
	{
		_clearColorBuffer();
		if(flags & ATTR_FLAG::DEPTH_BUFFER)
		{
			_clearDepthBuffer();
		}
	}

	void SoftwareRenderer::clearBuffer()
	{
		_clearColorBuffer();
		_clearDepthBuffer();
	}

	void SoftwareRenderer::_clearColorBuffer()
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

	void SoftwareRenderer::_clearDepthBuffer()
	{
		if(_attr & ATTR_FLAG::DEPTH_BUFFER)
		{
			memset(_fDepthBuffer, 0, sizeof(float) * iHeight * iWidth);
		}
	}

	void SoftwareRenderer::swapBuffer()
	{
		glWindowPos2d(0, 0);
		glDrawPixels(iWidth,iHeight, GL_RGB, GL_UNSIGNED_BYTE, _bColorBuffer);
	}

	void SoftwareRenderer::resizeViewPort(int width, int height)
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

	void SoftwareRenderer::drawArray(PolyObject *buffer, uint_32 offset, const Matrix4 &mat, uint_32 faceCount)
	{
		_cameraToProjectionTransform(buffer, offset, mat, faceCount);
		_ProjectionToScreenTransform(buffer, offset, faceCount);

		for(int index = offset; index < offset + faceCount; ++index)
		{
			drawTriangleSolid(buffer->triangleArray[index]);
		}
	}
	
	void SoftwareRenderer::drawElements(RenderList *buffer, GLushort *const indices, const Matrix4 &mat, uint_32 count)
	{
		uint_32 size = buffer->size();
		for(int idx = 0; idx < size; ++idx)
		{
			buffer->tranList[idx].p = mat * buffer->localList[idx].p;
			vec4f &v = buffer->tranList[idx].p;
			float inv_w = 1.0f / v.w;
			v.x = (v.x * inv_w + 1.0f) * iWidth * 0.5f;
			v.y = (v.y * inv_w + 1.0f) * iHeight * 0.5f;
			v.z = v.w;
			v.w = -inv_w;
		}

		for(int index = 0; index < count; )
		{
			Vertex &v0 = buffer->tranList[indices[index++]];
			Vertex &v1 = buffer->tranList[indices[index++]];
			Vertex &v2 = buffer->tranList[indices[index++]];
			drawTriangleSolid(v0, v1, v2);
		}
	}

	/*
	after this transform, the x, y, z param of each vertex will be in range[-1, 1],
	and w param will be -z
	*/
	void SoftwareRenderer::_cameraToProjectionTransform(PolyObject *buffer, uint_32 offset, const Matrix4 &projMat, uint_32 count)
	{
		for(int index = 0; index < count; ++index)
		{
			buffer->triangleArray[index].tranList[0].p = projMat * buffer->triangleArray[index].localList[0].p;
			buffer->triangleArray[index].tranList[1].p = projMat * buffer->triangleArray[index].localList[1].p;
			buffer->triangleArray[index].tranList[2].p = projMat * buffer->triangleArray[index].localList[2].p;
		}
	}
	/*
	after this transform, the x param of each vertex will be in range[0, ScreenWidth - 1],
	the y param will be in range[0, ScreenWidth - 1],
	the z param will be -z,(because the original z is no use, so we use it to record the z in world coordinate)
	and w param will be -1 / z
	*/
	void SoftwareRenderer::_ProjectionToScreenTransform(PolyObject *buffer, uint_32 offset, uint_32 count)
	{
		for (int index = offset; index < offset + count; ++index)
		{
			//actually inv_w is -1 / z, because z < 0, inv_w > 0
			float inv_w = 1.0f / buffer->triangleArray[index].tranList[0].p.w;
			buffer->triangleArray[index].tranList[0].p.x = (buffer->triangleArray[index].tranList[0].p.x * inv_w + 1.0f) * iWidth * 0.5f;
			buffer->triangleArray[index].tranList[0].p.y = (buffer->triangleArray[index].tranList[0].p.y * inv_w + 1.0f) * iHeight * 0.5f;
			buffer->triangleArray[index].tranList[0].p.z = buffer->triangleArray[index].tranList[0].p.w;
			buffer->triangleArray[index].tranList[0].p.w = -inv_w;

			inv_w = 1.0f / buffer->triangleArray[index].tranList[1].p.w;
			buffer->triangleArray[index].tranList[1].p.x = (buffer->triangleArray[index].tranList[1].p.x * inv_w + 1.0f) * iWidth * 0.5f;
			buffer->triangleArray[index].tranList[1].p.y = (buffer->triangleArray[index].tranList[1].p.y * inv_w + 1.0f) * iHeight * 0.5f;
			buffer->triangleArray[index].tranList[1].p.z = buffer->triangleArray[index].tranList[1].p.w;
			buffer->triangleArray[index].tranList[1].p.w = -inv_w;

			inv_w = 1.0f / buffer->triangleArray[index].tranList[2].p.w;
			buffer->triangleArray[index].tranList[2].p.x = (buffer->triangleArray[index].tranList[2].p.x * inv_w + 1.0f) * iWidth * 0.5f;
			buffer->triangleArray[index].tranList[2].p.y = (buffer->triangleArray[index].tranList[2].p.y * inv_w + 1.0f) * iHeight * 0.5f;
			buffer->triangleArray[index].tranList[2].p.z = buffer->triangleArray[index].tranList[2].p.w;
			buffer->triangleArray[index].tranList[2].p.w = -inv_w;
		}
	}

	void SoftwareRenderer::enable(ATTR_FLAG flag)
	{
		if(flag & ATTR_FLAG::DEPTH_BUFFER && !_fDepthBuffer)
		{
			_fDepthBuffer = GP_NEW_ARRAY(float, iWidth * iHeight);
		}

		_attr |= flag;
	}

	void SoftwareRenderer::disable(ATTR_FLAG flag)
	{
		_attr &= ~flag;
	}
}