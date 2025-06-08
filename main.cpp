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


int main() {
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