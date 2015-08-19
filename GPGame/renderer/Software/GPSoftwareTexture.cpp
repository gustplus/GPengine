#include "GPSoftwareTexture.h"

namespace GPEngine3D{
	TextureProtocol::TextureProtocol():
		_minFilter(SampleFilter::NEAREST),
		_magFilter(SampleFilter::NEAREST),
		_bytesPerColor(3),
		_mipmapLevel(0),
		_width(0),
		_height(0)
		{

		};

	TextureProtocol::~TextureProtocol()
	{
		
	}

	void TextureProtocol::setFilters(SampleFilter minFilter, SampleFilter magFilter)
	{
		_minFilter = minFilter;
		_magFilter = magFilter;
	}

	void Texture2D::generateMipmap()
	{
		if(!_img || _mipmapLevel)
		{
			return;
		}
		int width = _img->getWidth();
		int height = _img->getHeight();
		if(width > 512 || height > 512)
		{
			return;
		}
		int xTimes = log2Map[width];
		int yTimes = log2Map[height];

		if(xTimes || yTimes)
		{
			int mipLevel = xTimes > yTimes ? xTimes : yTimes;
			Image *origImg = _img;
			_img = (Image *)malloc(sizeof(origImg) * (mipLevel + 1));
			Image **imgs = (Image **)(_img);
			imgs[0] = origImg;

			int bytesPerColorMul2 = _bytesPerColor * 2;

			for (int level = 1; level <= mipLevel; ++level)
			{
				int bytePerRow = width * _bytesPerColor;
				width /= 2;
				height /= 2;
				byte *data = imgs[level - 1]->getData();
				byte *bytes = (byte *)malloc(width * height * _bytesPerColor);
				byte *leftTop;
				byte *rightTop;
				byte *leftBottom;
				byte *rightBottom;
				byte *curByte = bytes;
				for (int yCount = 0; yCount < height; ++yCount)
				{
					leftTop = data + bytePerRow * 2 * yCount;
					rightTop = leftTop + _bytesPerColor;
					leftBottom = leftTop + bytePerRow;
					rightBottom = rightTop + bytePerRow;
					for (int xCount = 0; xCount < width; ++xCount)
					{
						*(curByte++) = (*(leftTop++) + *(rightTop++) + *(leftBottom++) + *(rightBottom++)) / 4;
						*(curByte++) = (*(leftTop++) + *(rightTop++) + *(leftBottom++) + *(rightBottom++)) / 4;
						*(curByte++) = (*(leftTop++) + *(rightTop++) + *(leftBottom++) + *(rightBottom++)) / 4;
						leftTop += _bytesPerColor;
						rightTop += _bytesPerColor;
						leftBottom += _bytesPerColor;
						rightBottom += _bytesPerColor;
					}
				}
				imgs[level] = GP_NEW Image();
				imgs[level]->initWithBytes(bytes, width, height);
			}
			_mipmapLevel = mipLevel;
		}

	}


	Texture2D::~Texture2D()
	{
		disposeMipmap();
		GP_SAFE_DELETE(_img);
	}

	bool Texture2D::disposeMipmap()
	{
		if(_mipmapLevel > 0)
		{
			Image **imgs = (Image **)(_img);
			_img = imgs[0];
			for (int level = 1; level < _mipmapLevel; ++level)
			{
				GP_SAFE_DELETE(imgs[level]);
			}
			return true;
		}
		return false;
	}

	void Texture2D::setFilters(SampleFilter minFilter, SampleFilter magFilter)
	{
		TextureProtocol::setFilters(minFilter, magFilter);
	}

	void Texture2D::initWithBytes(byte *bytes, int width, int height, uint_32 bytesPerColor)
	{
		disposeMipmap();
		GP_SAFE_DELETE(_img);

		_img = GP_NEW Image();
		_img->initWithBytes(bytes, width, height);
		_width = width;
		_height = height;
		_mipmapLevel = 0;
	}

	Image *const Texture2D::getImage(int level) const
	{
		if(!_img)
		{
			return nullptr;
		}
		if(_mipmapLevel == 0)
		{
			return _img;
		}else{
			int lv = level > _mipmapLevel ? _mipmapLevel : level;
			Image **imgs = (Image **)(_img);
			return imgs[level];
		}
	}
}
