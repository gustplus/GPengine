#include "GPSoftwareTexture.h"

namespace GPEngine3D{
	TextureProtocol::TextureProtocol():
		_minFilter(SampleFilter::NEAREST),
		_magFilter(SampleFilter::NEAREST),
		_bIsmipmap(false),
		_width(0),
		_height(0),
		_data(nullptr)
		{};

	TextureProtocol::~TextureProtocol()
	{
		GP_SAFE_DELETE_ARRAY(_data);
	}

	void TextureProtocol::setFilters(SampleFilter minFilter, SampleFilter magFilter)
	{
		_minFilter = minFilter;
		_magFilter = magFilter;
	}



	void Texture2D::generateMipmap()
	{
		_bIsmipmap = true;
	}

	void Texture2D::setFilters(SampleFilter minFilter, SampleFilter magFilter)
	{
		TextureProtocol::setFilters(minFilter, magFilter);
	}

	void Texture2D::initWithBytes(byte *bytes, int width, int height)
	{
		this->_width = width;
        this->_height = height;
        this->_data = bytes;
	}
}
