#pragma once
class ColorRGBA
{
public:
	ColorRGBA(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a  = 1.0f);
	~ColorRGBA(void);


private:
	float fR;
	float fG;
	float fB;
	float fA;
};

