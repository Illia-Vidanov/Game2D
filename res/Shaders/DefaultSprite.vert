#version 460 compatibility
layout (location = 0) in vec2 vertex;

uniform mat3 mvp;

void main()
{
  gl_Position = vec4(mvp * vec3(vertex.xy, 1.0), 1.0);
}