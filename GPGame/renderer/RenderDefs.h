#ifndef _RENDER_DEFS_H_
#define _RENDER_DEFS_H_


enum class PixelFormat{
	RGB,
	RGBA
};

typedef struct Color3b {
	union
	{
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
		};
		unsigned char data[3];
	};
} Color3b;


#endif