#version 460 compatibility

in vec2 texture_coordinates;
out vec4 color;

uniform sampler2D image;

void main()
{
  color = texture(image, texture_coordinates);
}