#version 460 compatibility

out vec4 color;

uniform vec4 u_color;

void main()
{
  color = u_color;
}