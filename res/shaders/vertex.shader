#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color_in;
uniform vec4 u_Color;
out vec4 color;

void main()
{
	color = u_Color;
	gl_Position = position;
}
