#version 300 es
layout (location = 0) in vec3 position_vertex;
layout (location = 1) in vec2 tex_coord;

uniform mat4 cam;
uniform mat4 pos;
uniform mat4 ortho;
uniform mat4 scale;

out vec2 tex;

void main ()
{
	gl_Position = ortho * cam * pos * scale * vec4 (position_vertex, 1.0f);
	tex = tex_coord;
}