#ifndef _SOFTWARE_TEXTURE_H_
#define _SOFTWARE_TEXTURE_H_

#include "GPImage.h"

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
		~TextureProtocol();

		virtual void initWithBytes(byte *bytes, int width, int height) = 0;
		virtual void generateMipmap() = 0;
		virtual void setFilters(SampleFilter minFilter, SampleFilter magFilter) = 0;

		SampleFilter getMinFilter() {return _minFilter;};
		SampleFilter getMagFilter() {return _magFilter;};

		int getWidth() const {return _width;};
		int getHeight() const {return _height;};
		byte *const getData() const {
			return _data;
		}

	protected:
		bool _bIsmipmap;
		byte *_data;
		int _width;
		int _height;
	private:
		SampleFilter _minFilter;
		SampleFilter _magFilter;
	};

	class Texture2D : public TextureProtocol
	{
	public:
		void generateMipmap() override;
		void initWithBytes(byte *bytes, int width, int height) override;
		void setFilters(SampleFilter minFilter, SampleFilter magFilter) override;
	};
}

#endif