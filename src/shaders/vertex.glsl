#version 450 core
layout (location = 0) in vec3 aPos;
// out vec4 vertexColor;
// vertexColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
