#version 330

in vec3 vPos;
in vec3 vUV;
uniform mat4 vMVP;
out vec3 fUV;

void main()
{
	gl_Position = vMVP * vec4(vPos, 1);
	fUV = vUV;
}