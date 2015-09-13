#version 330

uniform mat4 vMVP;
uniform mat4 vMV;

in vec3 vPos;
in vec3 vNormal;

smooth out vec3 fNormal;
smooth out vec3 fPos;

void main(){
	gl_Position = vMVP * vec4(vPos, 1);
	fNormal = normalize(vNormal);
	fPos = vPos;
}