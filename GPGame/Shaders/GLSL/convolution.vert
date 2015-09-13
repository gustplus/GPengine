#version 330

in vec2 vPos;
out vec2 fUV;

void main() {
	gl_Position = vec4(vPos, 0, 1);
	fUV = (vPos + 1) * 0.5;
}