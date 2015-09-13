#version 330

in vec3 vPos;

uniform mat4 vMVP;

out vec3 fUV;

const float half = 0.5;
const int one = 1;

void main()
{
	vec4 pos = vMVP * vec4(vPos, one);
	float invW = one / pos.w;
	fUV = vec3( (pos.x * invW + one) * half * pos.w, (pos.y * invW + one) * half * pos.w, pos.w);
	gl_Position = pos;
}