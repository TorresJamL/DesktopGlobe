#version 330 core
layout (location = 0) in vec3 aPos, aColor;

out vec3 color;

void main()
{
	gl_position	= vec4(aPos.x, aPos.y, aPos.z, 1.0);
	color = aColor;
}