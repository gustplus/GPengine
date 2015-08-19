#ifndef _SOFTWARE_TEXTURE_H_
#define _SOFTWARE_TEXTURE_H_

#include "GPImage.h"
#include "gpmath.h"
#include "stdafx.h"

namespace GPEngine3D {
	enum class SampleFilter
	{
		NEAREST,
		LINEAR,
		NEAREST_NEAREST,
		NEAREST_LINEAR,
		LINEAR_NEAREST,
		LINEAR_LINEAR
	};

	class TextureProtocol
	{
	public:
		TextureProtocol();
		virtual ~TextureProtocol();

		virtual void initWithBytes(byte *bytes, int width, int height, uint_32 bytesPerColor) = 0;
		virtual void generateMipmap() = 0;
		virtual void setFilters(SampleFilter minFilter, SampleFilter magFilter) = 0;
		virtual bool disposeMipmap() = 0;


		SampleFilter getMinFilter() const {return _minFilter;};
		SampleFilter getMagFilter() const {return _magFilter;};

		int getMaxMipmapLevel() const {
			return _mipmapLevel;
		}

		int getWidth() const {return _width;}
		int getHeight()const {return _height;}
		
	protected:
		int _mipmapLevel;
		uint_32 _bytesPerColor;
		int _width;
		int _height;
	private:
		SampleFilter _minFilter;
		SampleFilter _magFilter;
	};

	class Texture2D : public TextureProtocol
	{
	public:
		Texture2D():TextureProtocol(),_img(nullptr){}
		virtual ~Texture2D();
		void generateMipmap() override;
		bool disposeMipmap() override;
		void initWithBytes(byte *bytes, int width, int height, uint_32 bytesPerColor = 3) override;
		void setFilters(SampleFilter minFilter, SampleFilter magFilter) override;
		Image *const getImage(int level = 0) const;
	private:
		Image *_img;
	};
}

#endif