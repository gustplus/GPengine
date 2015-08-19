#version 330

uniform mat4 uMVP;
in vec3 vPos;
void main()   
{                            		
   gl_Position = vec4(vPos,1);
}                      