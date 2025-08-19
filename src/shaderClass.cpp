#include <Windows.h>
#include <GL/glew.h>
#include "shaderClass.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw (errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	/*std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);*/
	const char* vertexSource = vertCode;
	const char* fragmentSource = fragCode;

	// Create the OpenGL vertex shader object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex shader source to vertex shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the vertex shader into machine code
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// Create the OpenGL fragment shader objecr and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach fragment shader source to fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the fragment shader to machine code
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// Create the shader program object and get its reference
	ID = glCreateProgram();
	// Attach the vertex and fragment shaders to the program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Link all shaders together into the shader program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	// Delete vertex and fradment shaders, individually they are now useless thus can be executed.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void Shader::Activate() {
	glUseProgram(ID);
}
void Shader::Delete() {
	glDeleteProgram(ID);
}
/*
Checks for compilation error with the shaders or shader program.
*/
void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;
		}
	}
}