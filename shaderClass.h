#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include "GLFW/glfw3.h"
#include <GLFW/glfw3native.h>

std::string get_file_contents(const char* filename);

class Shader
{
public :
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
private :
	void compileErrors(unsigned int shader, const char* type);
	const char* vertCode = R"(#version 330 core 
// Pos / Coordinates 
layout(location = 0) in vec3 aPos; 
// Colors 
layout(location = 1) in vec3 aColor; 
// Texture coordinates 
layout(location = 2) in vec2 aTex; 

out vec3 color; 
out vec2 texCoord; 

uniform mat4 camMatrix; 
uniform mat4 model; 

void main() 
{
	gl_Position = camMatrix * model * vec4(aPos, 1.0); 
	color = aColor;
	texCoord = aTex; 
}
)";
	const char* fragCode = R"(#version 330 core 	 
out vec4 FragColor;
in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
}
)";
};

#endif 
