#version 330

in vec2 vTextureCoord;
uniform sampler2D sTexture;
void main()                         
{                      
   gl_FragColor =  texture2D(sTexture, vTextureCoord);
   //gl_FragColor =  vec4(0.5f);
}              