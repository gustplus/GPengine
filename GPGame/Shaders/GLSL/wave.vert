#version 330

const float PI = 3.14159;
const float POWER = 2;
const float f = 0.125;

uniform mat4 vMVP;
uniform float time;
in vec3 vPos;

smooth out float y;

void main()
{
	float len = length(vPos);
	y = POWER * sin(-PI * f * len + time);
	gl_Position = vMVP * vec4(vPos.x, y, vPos.z, 1);
}