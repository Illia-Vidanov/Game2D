#version 460 compatibility
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat3 mvp;

void main()
{
  TexCoords = vertex.zw;
  gl_Position = vec4(mvp * vec3(vertex.xy, 1.0), 1.0);
}