#version 460 compatibility
layout (location = 0) in vec2 vertex;

out vec2 texture_coordinates;

uniform mat3 mvp;

void main()
{
  texture_coordinates = vec2(vertex.x + 0.5, vertex.y + 0.5);
  gl_Position = vec4(mvp * vec3(vertex.xy, 1.0), 1.0);
}