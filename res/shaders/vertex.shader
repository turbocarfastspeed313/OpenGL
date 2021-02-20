#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color_in;
out vec4 color;

void main()
{
	color = color_in;
	gl_Position = position;
}
