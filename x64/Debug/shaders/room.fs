#version 330 core
out vec4 FragColor;
uniform vec4 triangle_color;
void main()
{
   FragColor = triangle_color;
}