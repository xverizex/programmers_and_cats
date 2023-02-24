#version 300 es
layout (location = 0) in vec3 position_vertex;

uniform mat4 pos;
uniform mat4 ortho;

out vec2 tex;

void main ()
{
	gl_Position = ortho * pos * vec4 (position_vertex, 1.0f);
}