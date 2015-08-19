#version 330

smooth in float y;
const float f = 0.5;

void main()
{
	float c = (y + 1) * f;
	gl_FragColor = vec4(c, 0.9, 0.9, 1);
}