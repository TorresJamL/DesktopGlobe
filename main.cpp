#include <Windows.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 corex\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_position	= vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 corex\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.2f, 1.0f);\n"
"}\n\0";


// FreeCodeCamp vid: https://youtu.be/45MIykWJ-C4
int main() {
	glfwInit();
	// Window hints to tell the window what it should be. 
	// This is because all good windows should follow social norms.
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // Set GLFW_TRUE to GLFW_FALSE for the window to be completely transparent.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // TRUE to resize, false otherwise. 
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // C'mon, i dont need to say it.

	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};
	// Creates the transparent window.
	GLFWwindow* wnd = glfwCreateWindow(500, 500, "Transparent", NULL, NULL);

	// Error check for if the window doesn't load, aka it equals null.
	if (!wnd) { glfwTerminate(); return -1; }

	// I... honestly don't know. Puts the window into context. Whatever that means.
	glfwMakeContextCurrent(wnd);
	glewExperimental = GL_TRUE;
	glewInit();

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
		

		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

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