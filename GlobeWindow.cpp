#include "GlobeWindow.h"
#include <Windows.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

/*
 * Prints to Debug
 */
void print(std::string str) {
	str += "\n";
	std::wstring wstr(str.begin(), str.end());
	OutputDebugString(wstr.c_str());
}

GlobeWindow::~GlobeWindow() {
	if (GLFWwnd)
		glfwDestroyWindow(GLFWwnd);

	glfwTerminate();
}

GlobeWindow::GlobeWindow(){
	this->title = "Transparent Globe Win";
	GLFWwnd = CreateGLFW_Window(title, NULL, NULL);
	m_hwnd = glfwGetWin32Window(GLFWwnd);
}

GlobeWindow::GlobeWindow(const char* title) {
	this->title = title;
	GLFWwnd = CreateGLFW_Window(title, NULL, NULL);
	m_hwnd = glfwGetWin32Window(GLFWwnd);
}

void GlobeWindow::ShouldClose() {
	if (glfwGetKey(GLFWwnd, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(GLFWwnd, 1);
	}
}
void GlobeWindow::updateMousePassThrough() {
	double xpos, ypos;
	glfwGetCursorPos(GLFWwnd, &xpos, &ypos);

	// Flip Y because OpenGL origin is bottom-left, Windows is top-left
	ypos = height - ypos;

	// Read depth value at mouse position
	float depth = 1.0f;
	glReadPixels((int)xpos, (int)ypos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	// Check if mouse is over a drawn object (like the sphere)
	bool isOverSphere = (depth < 1.0f); // 1.0 = background

	// Update WS_EX_TRANSPARENT accordingly
	LONG exStyle = GetWindowLong(m_hwnd, GWL_EXSTYLE);
	if (isOverSphere) {
		SetWindowLong(m_hwnd, GWL_EXSTYLE, exStyle & ~WS_EX_TRANSPARENT); // allow clicks
		print("Allowing Clicks");
	}
	else {
		SetWindowLong(m_hwnd, GWL_EXSTYLE, exStyle | WS_EX_TRANSPARENT);  // pass through
		print("Passing Through");
	}
}

void GlobeWindow::SetWindowStyles() {
	// Gets rid of the white bar background behind the window's name, aka the frame.
	MARGINS margins = { -1 };
	// Extends the window frame into the client area.
	DwmExtendFrameIntoClientArea(m_hwnd, &margins);

	// More code to make it transparent.
	LONG style = GetWindowLong(m_hwnd, GWL_EXSTYLE);
	style |= WS_EX_LAYERED; // | WS_EX_TRANSPARENT
	SetWindowLong(m_hwnd, GWL_EXSTYLE, style);


	SetLayeredWindowAttributes(m_hwnd, 0, 255, LWA_ALPHA);
}


BOOL CALLBACK GlobeWindow::EnumWindowsZ(HWND hwnd, LPARAM lParam) {
	if (!IsWindowVisible(hwnd)) return true;

	// Cast lParam back to GlobeWindow*
	GlobeWindow* self = reinterpret_cast<GlobeWindow*>(lParam);
	self->topLevelWindows.push_back(hwnd);
	return true;
}

/*
 * retrieves the window at the provided z-order. 0 being the top most window.
 * the size-1 is the program manager and must be the last one in the list.
 * @param z_order the z index of the window you are looking for.
 */
HWND GlobeWindow::RetrieveWindowAtZ(int z_order) {
	topLevelWindows.clear();
	EnumWindows(EnumWindowsZ, reinterpret_cast<LPARAM>(this));
	return topLevelWindows[z_order];
}

void GlobeWindow::SendHWND_ToZ(int z) {
	SetWindowPos(m_hwnd, RetrieveWindowAtZ(z - 1), 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

GLFWwindow* GlobeWindow::CreateGLFW_Window(
	const char* title,
	GLFWmonitor* monitor,
	GLFWwindow* share) {

	if (!glfwInit()) {
		throw std::runtime_error("GLFW initialization failed");
	}

	// Window hints to tell the window what it should be. 
	// This is because all good windows should follow social norms.
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Set GLFW_TRUE to GLFW_FALSE for the window to be completely transparent.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // TRUE to resize, false otherwise. 
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // C'mon, i dont need to say it.

	// Creates the transparent window.
	GLFWwindow* wnd = glfwCreateWindow(500, 500, title, monitor, share);

	if (!wnd) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(wnd);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		// If glew is not ok, terminate everything. On sight. Otherwise, continue on.
		glfwDestroyWindow(wnd);
		glfwTerminate();

		throw std::runtime_error("GLEW initialization failed");
	}
	glfwGetFramebufferSize(wnd, &width, &height);

	return wnd;
}

std::string GlobeWindow::GetWindowTitle(HWND hwnd) {
	char title[256];
	int length = GetWindowTextA(hwnd, title, sizeof(title));
	if (length > 0)
		return std::string(title);
	else
		return std::string("[No Title]");
}