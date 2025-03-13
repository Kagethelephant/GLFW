#version 450 core
out vec4 FragColor;
uniform vec4 vertexColor;
// in vec4 vertexColor;
void main()
{
   FragColor = vertexColor;
}
