#version 330

in vec2 vPos;

out vec2 UV;

void main()
{
	gl_Position = vec4(vPos.xy, 0, 1);
	UV = vPos;
}