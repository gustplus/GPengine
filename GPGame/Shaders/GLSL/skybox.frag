#version 330

in vec3 fUV;
uniform samplerCube tex;
layout(location = 0) out vec4 vFragColor;

void main()
{
	vFragColor = texture(tex, fUV);
}