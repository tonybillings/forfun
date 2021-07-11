#version 300 es

layout(location = 0) in vec3 inPos;
in vec4 inCol;

uniform mat4 inMVP;

out vec4 color;

void main()
{
	gl_Position = inMVP * vec4(inPos, 1.0);
	color = inCol;
}