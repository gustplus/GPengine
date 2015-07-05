#ifndef _COLOR_RGBA_H_
#define _COLOR_RGBA_H_

namespace GPEngine3D{
class ColorRGBA
{
public:
	ColorRGBA(float fr = 0.0f, float fg = 0.0f, float fb = 0.0f, float fa  = 1.0f);
	~ColorRGBA(void);

	float r;
	float g;
	float b;
	float a;
};
}

#endif