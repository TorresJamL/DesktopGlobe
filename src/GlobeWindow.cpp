#include "GlobeWindow.h"
#include <Windows.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>

#include "Config.h"

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

/* Prints to Debug */
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
	desiredZ_order = (Config::desiredZ_order == -1) ? getInitialZ_Order() : Config::desiredZ_order;
}
/**
 * @brief Constuctor with the option of a title.
 * @param title: The title of the window.
 */
GlobeWindow::GlobeWindow(const char* title) {
	this->title = title;
	GLFWwnd = CreateGLFW_Window(title, NULL, NULL);
	m_hwnd = glfwGetWin32Window(GLFWwnd);
}
/* Constantly Update window.*/
void GlobeWindow::Update() {
	ShouldClose();
	updateMousePassThrough();
	SendHWND_ToZ(desiredZ_order);
}
/* If the esc-key is pressed, exit the window. */
void GlobeWindow::ShouldClose() {
	if (glfwGetKey(GLFWwnd, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(GLFWwnd, 1);
	}
}
/**
 * @brief Checks the depth of the screen on the window for changes, 
 *        if found, makes the window interactable. 
 *        Otherwise clicks will pass through the window.
 */
void GlobeWindow::updateMousePassThrough() {
	double xpos, ypos;
	glfwGetCursorPos(GLFWwnd, &xpos, &ypos);

	// Flip Y because OpenGL origin is bottom-left, Windows is top-left
	ypos = height - ypos;

	float depth = 1.0f;
	glReadPixels((int)xpos, (int)ypos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	bool isOverSphere = (depth < 1.0f); // 1.0 = background
	
	LONG exStyle = GetWindowLong(m_hwnd, GWL_EXSTYLE);
	exStyle |= WS_EX_LAYERED;
	if (isOverSphere) {
		exStyle &= ~WS_EX_TRANSPARENT; // allow clicks
		//print("Allowing Clicks");
	}
	else {
		exStyle |= WS_EX_TRANSPARENT; // pass through
		//print("Passing Through");
	}
	SetWindowLong(m_hwnd, GWL_EXSTYLE, exStyle);
}
/* @brief Sets the window style to make it transparent. */
void GlobeWindow::SetWindowStyles() {
	// Gets rid of the white bar background behind the window's name, aka the frame.
	MARGINS margins = { -1 };
	// Extends the window frame into the client area.
	DwmExtendFrameIntoClientArea(m_hwnd, &margins);

	// More code to make it transparent.
	LONG exStyle = GetWindowLong(m_hwnd, GWL_EXSTYLE);
	SetWindowLong(m_hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);

	SetLayeredWindowAttributes(m_hwnd, 0, 255, LWA_ALPHA);
}
/**
 * @brief Callback function that collects visible top-level windows.
 *        This function is used with EnumWindows to collect all visible HWNDs
 *        into the GlobeWindow's topLevelWindows vector.
 * 
 * @param hwnd:   A handle to the window being enumerated.
 * @param lParam: A pointer cast as LPARAM, used to pass the GlobeWindow instance.
 * @return        TRUE to continue enumeration, FALSE to stop.
 */
BOOL CALLBACK GlobeWindow::EnumWindowsZ(HWND hwnd, LPARAM lParam) {
	if (!IsWindowVisible(hwnd)) return true;

	// Cast lParam back to GlobeWindow*
	GlobeWindow* self = reinterpret_cast<GlobeWindow*>(lParam);
	self->topLevelWindows.push_back(hwnd);
	return true;
}
/**
 * @brief Retrieves the window at the provided z-order. 0 being the top most window. 
          The size-1 is the program manager and must be the last one in the list.
 * @param  z_order the z index of the window you are looking for.
 * @return The HWND window at z_order
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
/* @return The inital integer Z order */
int GlobeWindow::getInitialZ_Order() {
	SetWindowPos(m_hwnd, HWND_BOTTOM, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	topLevelWindows.clear();
	EnumWindows(EnumWindowsZ, reinterpret_cast<LPARAM>(this));

	for (int i = topLevelWindows.size() - 1; i > 0; i--) {
		if (topLevelWindows[i] == m_hwnd) {
			return i;
		}
	}
	return -1;
}
/**
 * @brief Creates a GLFW window.
 *
 * @param title:   The window title.
 * @param monitor: The monitor for fullscreen mode, or NULL for windowed.
 * @param share:   A window to share OpenGL context with, or NULL.
 * @return         A pointer to the created GLFWwindow, or NULL on failure.
 */
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