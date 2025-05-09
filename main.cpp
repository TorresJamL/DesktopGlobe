#include <Windows.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#include <iostream>

// Vertex shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_position	= vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// Fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.2f, 1.0f);\n"
"}\n\0";
GLFWwindow* createGLFW_Window(
		int width, 
		int height, 
		const char *title, 
		GLFWmonitor* monitor, 
		GLFWwindow* share) {	
	glfwInit();
	// Window hints to tell the window what it should be. 
	// This is because all good windows should follow social norms.
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // Set GLFW_TRUE to GLFW_FALSE for the window to be completely transparent.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // TRUE to resize, false otherwise. 
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // C'mon, i dont need to say it.

	// Creates the transparent window.
	GLFWwindow* wnd = glfwCreateWindow(500, 500, "Transparent", NULL, NULL);
	return wnd;
}
GLuint createGL_ShaderProgram() {
	// Create the OpenGL vertex shader object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex shader source to vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);	
	// Compile the vertex shader into machine code
	glCompileShader(vertexShader);

	// Create the OpenGL fragment shader objecr and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach fragment shader source to fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the fragment shader to machine code
	glCompileShader(fragmentShader);

	// Create the shader program object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the vertex and fragment shaders to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Link all shaders together into the shader program
	glLinkProgram(shaderProgram);

	// Delete vertex and fradment shaders, individually they are now useless thus can be executed.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
// FreeCodeCamp vid: https://youtu.be/45MIykWJ-C4
int main() {
	GLFWwindow* wnd = createGLFW_Window(500, 500, "Transparent", NULL, NULL);

	// Error check for if the window doesn't load, aka it equals null.
	if (!wnd) { glfwTerminate(); return -1; }

	// I... honestly don't know. Puts the window into context. Whatever that means.
	glfwMakeContextCurrent(wnd);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		glfwDestroyWindow(wnd);
		glfwTerminate();
		return -1;
	}

	// Define vertices for a equi-Triangle
	GLuint shaderProgram = createGL_ShaderProgram();
	
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Inner down	
	};
	GLuint indices[] = {
		0, 3, 5, // lower left tri
		3, 2, 4, // lower right tri
		5, 4, 1 // upper tri
	};

	// Create reference containers for Vertex Array Object and Vertex Buffer Object
	GLuint VAO, VBO, EBO;	

	// VAO Must be generated BEFORE the VBO
	// Generates the VAO, VBO, and EBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	// Make VAO the current vertex array object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifing it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Bind the EBO, specifying it's a GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Introduce indices into the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that openGl knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the attribute so openGL kows how to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO, VAO and EBO to 0 so we don't accidently modify the VBO, VAO, EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Gets Win32 window from the glfw window. A lot of windowing happening here.
	HWND hwnd = glfwGetWin32Window(wnd);

	// Extends the window frame into the client area.
	// Gets rid of the white bar background behind the window's name, aka the frame.
	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margins);

	// More code to make it transparent.
	LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
	style |= WS_EX_LAYERED | WS_EX_TRANSPARENT;
	SetWindowLong(hwnd, GWL_EXSTYLE, style);

	SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(wnd)) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw space
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

	// Delete all the objects created. 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(wnd);
	glfwTerminate();
	return 0;
}
/* 
Main function that can be called without opening a console window. 
@param hInstance: Does smth, idk yet.
@param theOtherOnes: Do other things i guess?
@returns (int), calls the int main(){ ... } function which contains all operations/algoritms. 0 if successful, -1 if not.
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	return main();
}