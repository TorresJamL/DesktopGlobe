#include <Windows.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#include <iostream>
#include "shaderClass.h"
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"
#include "ElementBufferObject.h"

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
	GLfloat vertices[] = {
	//  |<--------------Coordinates-------------->|    |<-----Colors----->|
		-0.5f, -0.5f * float(sqrt(3)) / 3,    0.0f,		0.8f, 0.3f,  0.02f,		// Lower left corner
		 0.5f, -0.5f * float(sqrt(3)) / 3,    0.0f,		0.8f, 0.3f,  0.02f,		// lower right corner
		 0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,		1.0f, 0.6f,  0.32f,		// Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,		0.9f, 0.45f, 0.17f,		// Inner left
		 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,		0.9f, 0.45f, 0.17f,		// Inner right
		 0.0f, -0.5f * float(sqrt(3)) / 3,    0.0f,		0.8f, 0.3f,  0.02f		// Inner down	
	};
	GLuint indices[] = {
		0, 3, 5, // lower left tri
		3, 2, 4, // lower right tri
		5, 4, 1 // upper tri
	};
	// Creates shader object
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VertexArrayObject VAO1;
	VAO1.Bind();

	// Creates Vertex Buffer Object and links it to vertices
	VertexBufferObject VBO1(vertices, sizeof(vertices));
	// Creates Element Buffer Object and links it to indices
	ElementBufferObject EBO1(indices, sizeof(indices)); 

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all objects to prevent modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Used to scale shapes made by a float value or smth
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

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

		//-- AI generated -- // Resizes the drawn shape to match the aspect ratio of the window
		int width, height;
		glfwGetFramebufferSize(wnd, &width, &height); 
		float aspect = width / (float)height;
		//-^ AI generated -^ //
			
		// Draw space
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		//-- AI generated -- //
		GLuint aspectLoc = glGetUniformLocation(shaderProgram.ID, "aspect");
		glUniform1f(aspectLoc, aspect);
		//-^ AI generated -^ //

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		
		// End of draw space.
		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

	// Delete all the objects created. 
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
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
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	return main();
}