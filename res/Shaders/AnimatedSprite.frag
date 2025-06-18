#version 460 compatibility

in vec2 texture_coordinates;
out vec4 color;

uniform sampler2D image;
uniform ivec2 sprite_index;
uniform float atlas_step;

void main()
{
  vec4 texture_color = texture(image, (texture_coordinates + sprite_index) * atlas_step);
  // Test for outlining
  if(texture_color.a == 0.0)
    discard;
  color = texture_color;
}