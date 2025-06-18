#version 460 compatibility

in vec2 texture_coordinates;
out vec4 color;

uniform sampler2D image;

void main()
{
  vec4 texture_color = texture(image, texture_coordinates);
  // Test for outlining
  if(texture_color.a == 0)
    discard;
  color = texture_color;
}