#version 330

uniform sampler2D tex;
uniform float kernel[9];
uniform float power;
in vec2 fUV;
layout(location=0) out vec4 Frag_Color;
void main() {
	int index = 9;
	vec4 color = vec4(0);
	for(int row = -1; row <= 1; ++row)
	{
		for(int col = -1; col <= 1; ++col)
		{
			color += texture(tex, fUV + vec2(row/800.0, col/480.0)) * kernel[(row + 1) * 3 + 1 + col]; 
		}
	}
	color /= 9.0;
	Frag_Color = (color + texture(tex, fUV)) * power;
}