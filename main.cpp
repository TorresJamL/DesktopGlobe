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
#include "GlobeWindow.h"

///*
// * Prints to Debug
// */
//void print(std::string str) {
//	str += "\n";
//	std::wstring wstr(str.begin(), str.end());
//	OutputDebugString(wstr.c_str());
//}
//std::string GetWindowTitle(HWND hwnd) {
//	char title[256];
//	int length = GetWindowTextA(hwnd, title, sizeof(title));
//	if (length > 0)
//		return std::string(title);
//	else
//		return "[No Title]";
//}

/*
Makes a GLFWwindow*
@returns GLFWwindow* : A transparent, maximized, resizable, undecorated window
*/
//static GLFWwindow* createGLFW_Window(
//		int width, 
//		int height, 
//		const char *title, 
//		GLFWmonitor* monitor, 
//		GLFWwindow* share) {	
//	glfwInit();
//	// Window hints to tell the window what it should be. 
//	// This is because all good windows should follow social norms.
//	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
//	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Set GLFW_TRUE to GLFW_FALSE for the window to be completely transparent.
//	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // TRUE to resize, false otherwise. 
//	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // C'mon, i dont need to say it.
//
//	// Creates the transparent window.
//	GLFWwindow* wnd = glfwCreateWindow(500, 500, "Transparent", NULL, NULL);
//	return wnd;
//}

//std::vector<HWND> topLevelWindows;
//BOOL CALLBACK EnumWindowsZ(HWND hwnd, LPARAM lParam) {
//	if (!IsWindowVisible(hwnd)) return TRUE; // Skip invisible windows
//
//	topLevelWindows.push_back(hwnd);
//	return TRUE;
//}
//
///*
// * Retrieves the window at the provided z-order. 0 being the top most window.
// * The size-1 is the program manager and must be the last one in the list. 
// * @param z_Order The z index of the window you are looking for.
// */
//HWND static RetrieveWindowAtZ(int z_Order) {
//	topLevelWindows.clear();
//	EnumWindows(EnumWindowsZ, 0);
//	return topLevelWindows[z_Order];
//}

//void sendHWND_ToZ(HWND hwnd, int z) {
//	SetWindowPos(hwnd, RetrieveWindowAtZ(z-1), 0, 0, 0, 0,
//		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
//}

int main() {
	//GLFWwindow* wnd = createGLFW_Window(500, 500, "Transparent", NULL, NULL);
	//// Error check for if the window doesn't load, aka it equals null.
	//if (!wnd) { glfwTerminate(); return -1; }

	//// Gets Win32 window from the glfw window. A lot of windowing happening here.
	//HWND hwnd = glfwGetWin32Window(wnd);
	//
	//// I... honestly don't know. Puts the window into context. Whatever that means.
	//glfwMakeContextCurrent(wnd);
	//glewExperimental = GL_TRUE;
	//if(glewInit() != GLEW_OK) {
	//	// If glew is not ok, terminate everything. On sight. Otherwise, continue on.
	//	std::cerr << "Failed to initialize GLEW" << std::endl;
	//	glfwDestroyWindow(wnd);
	//	glfwTerminate();
	//	return -1;
	//}
	//int width, height;
	//glfwGetFramebufferSize(wnd, &width, &height);

	GlobeWindow wnd;

	Shader shaderProgram("default.vert", "default.frag");
	Sphere sphere(0.2f, 144, 72, wnd.width, wnd.height, 45.0f, 3.0f, 3.0f);

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

	
	wnd.SetWindowStyles();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	
	Camera camera(wnd.width, wnd.height, glm::vec3(0.0f, 0.0f, 3.0f));

	double deltaTime = 0.0;
	double lastFrame = 0.0;
	double currentFrame = 0.0;
	int rotater = 0;

	camera.freeCamera = false;
	
	while (!glfwWindowShouldClose(wnd.GLFWwnd)) {
		
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// Draw space
		shaderProgram.Activate();
	

		camera.Inputs(wnd.GLFWwnd, (float)deltaTime);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		 
		// Rotates the sphere around the z-axis
		sphere.Draw(
			shaderProgram,
			sphere.orientation,
			sphere.rotation,
			sphere.orientationAxis,
			sphere.rotationAxis
		);

		sphere.Inputs(wnd.GLFWwnd, (float)deltaTime, wnd.width, wnd.height, camera.position, camera.view, camera.projection);
		sphere.Rotate(wnd.GLFWwnd, (float)deltaTime, wnd.width, wnd.height, camera.position, camera.view, camera.projection);

		// Binds texture so that is appears in rendering
		image.Bind();

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphere.getIndices().size()), GL_UNSIGNED_INT, 0);

		glFlush(); 

		wnd.updateMousePassThrough();
		// End of draw space.
		wnd.ShouldClose();

		glfwSwapBuffers(wnd.GLFWwnd);
		glfwPollEvents();
	}

	// Delete all the objects created. "Before creation, comes destruction" - Beerus... Should've learned to code.
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	image.Delete();
	wnd.~GlobeWindow();
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