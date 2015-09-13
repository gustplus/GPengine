#version 330

in vec3 fUV;

uniform sampler2D tex;

layout(location=0) out vec4 Frag_Color;

const int one = 1;

void main()
{
	float invW = one / fUV.z;
	vec2 uv = vec2( fUV.x * invW, fUV.y * invW);
	Frag_Color = texture(tex, uv) * 0.97;
}