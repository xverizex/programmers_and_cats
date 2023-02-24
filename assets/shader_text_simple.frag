#version 300 es

precision mediump float;

in vec2 TexCoords;
layout (location = 0) out vec4 color;

uniform sampler2D stext;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4 (1.0, 1.0, 1.0, texture (stext, TexCoords).r);
	color = vec4 (textColor, 1.0) * sampled;
}
