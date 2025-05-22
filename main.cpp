#include <Windows.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>

#include <iostream>
#include <vector>

#include "stb/stb_image.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderClass.h"
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"
#include "ElementBufferObject.h"
#include "Texture.h"
#include "Camera.h"
#include "Sphere.h"

// Define vertices for a equi-Triangle made of three triangles with an open space in the middle that happens to be triangular. !THIS EX, DOES NOT HAVE TEXTURE!
//GLfloat vertices[] = {
//	//  |<--------------Coordinates-------------->|    |<-----Colors----->|
//		-0.5f, -0.5f * float(sqrt(3)) / 3,    0.0f,		0.8f, 0.3f,  0.02f,		// Lower left corner
//		 0.5f, -0.5f * float(sqrt(3)) / 3,    0.0f,		0.8f, 0.3f,  0.02f,		// lower right corner
//		 0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,		1.0f, 0.6f,  0.32f,		// Upper corner
//		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,		0.9f, 0.45f, 0.17f,		// Inner left
//		 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,		0.9f, 0.45f, 0.17f,		// Inner right
//		 0.0f, -0.5f * float(sqrt(3)) / 3,    0.0f,		0.8f, 0.3f,  0.02f		// Inner down	
//};
//GLuint indices[] = {
//	0, 3, 5, // lower left tri
//	3, 2, 4, // lower right tri
//	5, 4, 1 // upper tri
//};

// Defines vertex coordinates for a square, and its colors too. 
// Nothing really special about this one unfortunately but, hey, its fair to be square. :thumbsup:
// This also generates the coordinate system for the texture and how it applies to the vertices. kinda. 
// I explained it poorly.
//GLfloat vertices[] = {
////  |<--Coordinates-->|    |<----Colors---->|  |<Texture Coords>|
//	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // lower left corner
//	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // upper left corner
//	 0.5f,	0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // upper right corner
//	 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f, // lower right corner
//};
//// Define the array of indices (Know as an element array). This tells OpenGL which vertices to connect and in what order to form primitives.
//// Without indices you repeat shared vertices for each triangle, making it good for efficiency!
//GLuint indices[] = {
//	0, 2, 1, // upper triangle
//	0, 3, 2, // lower tri-angles (im so funny.) -> (correction: I'm so funny!)
//};

/*
TIME FOR 3D BABY, WHOOOO
@1:00:00 in, drawing a pyramid. Sphere is SO close yall.
*/
// Vertices coordinates
//GLfloat vertices[] = {
////	|<--Coordinates-->|    |<------Colors----->|   |<Texture Coords>|
//	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
//	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
//	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
//	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
//	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
//};
//
//// Indices for vertices order
//GLuint indices[] = {
//	0, 1, 2,
//	0, 2, 3,
//	0, 1, 4,
//	1, 2, 4,
//	2, 3, 4,
//	3, 0, 4
//};

Sphere sphere(0.5f, 144, 72);

/*
Makes a GLFWwindow*
@returns GLFWwindow* : A transparent, maximized, resizable, undecorated window
*/
static GLFWwindow* createGLFW_Window(
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

#ifdef _WIN32
// BE SKEPTICAL, MAY OR MAY NOT WORK. 
// Currently creates nigh-impossible to remove wallpaper windows which are annoying.
bool attachToDesktopWorkerW(HWND hwnd) {
	HWND progman = FindWindow(L"Progman", nullptr);
	if (!progman) return false;

	// Force creation of WorkerW
	SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);

	HWND workerw = nullptr;

	EnumWindows([](HWND top, LPARAM lParam) -> BOOL {
		HWND shellView = FindWindowEx(top, nullptr, L"SHELLDLL_DefView", nullptr);
		if (shellView) {
			// Found the desktop icons container — next sibling WorkerW is what we want
			HWND* out = reinterpret_cast<HWND*>(lParam);
			*out = FindWindowEx(nullptr, top, L"WorkerW", nullptr);
			return FALSE;
		}
		return TRUE;
		}, reinterpret_cast<LPARAM>(&workerw));

	if (workerw) {
		SetParent(hwnd, workerw);
		OutputDebugString(L"[attachToDesktopWorkerW] Success.\n");
		return true;
	}
	else {
		OutputDebugString(L"[attachToDesktopWorkerW] Failed.\n");
		return false;
	}
}
#endif


// FreeCodeCamp vid: https://youtu.be/45MIykWJ-C4
int main() {
	GLFWwindow* wnd = createGLFW_Window(500, 500, "Transparent", NULL, NULL);
	// Error check for if the window doesn't load, aka it equals null.
	if (!wnd) { glfwTerminate(); return -1; }

	// Gets Win32 window from the glfw window. A lot of windowing happening here.
	HWND hwnd = glfwGetWin32Window(wnd);
	// Attach the window to the desktop so it is behind the apps but infront of desktop.
	//  Currently extremely buggy and does not work.
	//attachToDesktopWorkerW(hwnd);

	// I... honestly don't know. Puts the window into context. Whatever that means.
	glfwMakeContextCurrent(wnd);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		// If glew is not ok, terminate everything. On sight. Otherwise, continue on.
		std::cerr << "Failed to initialize GLEW" << std::endl;
		glfwDestroyWindow(wnd);
		glfwTerminate();
		return -1;
	}
	int width, height;
	glfwGetFramebufferSize(wnd, &width, &height);
	// Creates shader obj~ect
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VertexArrayObject VAO1;
	VAO1.Bind();

	// Creates Vertex Buffer Object and links it to vertices
	VertexBufferObject VBO1(sphere.getVertices().data(), sphere.getVertices().size() * sizeof(GLfloat));
	// Creates Element Buffer Object and links it to indices
	ElementBufferObject EBO1(sphere.getIndices().data(), sphere.getIndices().size() * sizeof(GLuint));
	
	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all objects to prevent modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	Texture image("newEarth.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	image.texUnit(shaderProgram, "tex0", 0);

	
	// Extends the window frame into the client area.
	// Gets rid of the white bar background behind the window's name, aka the frame.
	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margins);

	// More code to make it transparent.
	LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
	style |= WS_EX_LAYERED; // | WS_EX_TRANSPARENT
	SetWindowLong(hwnd, GWL_EXSTYLE, style);

	SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 3.0f));

	double deltaTime = 0.0;
	double lastFrame = 0.0;
	int rotater = 0;

	camera.freeCamera = false;
	while (!glfwWindowShouldClose(wnd)) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// Draw space
		shaderProgram.Activate();
	
		// Rotates the sphere around the z-axis
		sphere.Draw(
			shaderProgram, 
			-90.0f, rotater * 1.0f,
			glm::vec3(1.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f))
		);

		camera.Inputs(wnd, deltaTime, sphere);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		
		// Binds texture so that is appears in rendering
		image.Bind();

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphere.getIndices().size()), GL_UNSIGNED_INT, 0);
		
		// End of draw space.
		if (glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(wnd, 1);
		}
		glfwSwapBuffers(wnd);
		glfwPollEvents();
		rotater++;
	}

	// Delete all the objects created. "Before creation, comes destruction" - Beerus... Should've learned to code.
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	image.Delete();
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