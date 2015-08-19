#version 330

uniform float power;
in vec2 UV;
uniform sampler2D tex;

layout(location = 0) out vec4 vFragColor;

void main()
{
	vec2 uv = UV * 0.5;
	float angle = atan(uv.y, uv.x);
	float radius = length(uv);
	angle += (0.5 - radius) * power;
	vec2 shift = radius * vec2(cos(angle), sin(angle));
	vFragColor = texture(tex, shift + 0.5);
}