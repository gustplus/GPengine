#version 330
in vec3 VertexPosition;
in vec3 VertexColor;
uniform mat3 ms;
out vec3 Color;
void main()
{
    Color = VertexColor;
    gl_Position = vec4(VertexPosition,1.0);
}
