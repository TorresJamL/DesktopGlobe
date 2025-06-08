#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>
// TODO: Add a desired Z order, that the main loop will force the window to be at if it isn't at it.
//			Initialize this desiredZ_Order by getting the Z order after forcing the window to the bottom!
class GlobeWindow {
public:
	const char* title;
	GLFWwindow* GLFWwnd;
	HWND m_hwnd;
	int width, height;
	int desiredZ_order;

	~GlobeWindow();
	GlobeWindow();
	GlobeWindow(const char* title);

	int getInitialZ_Order();

	void ShouldClose();
	void updateMousePassThrough();
	void SetWindowStyles();
	void SendHWND_ToZ(int z);

	static BOOL CALLBACK EnumWindowsZ(HWND hwnd, LPARAM lParam);
	HWND RetrieveWindowAtZ(int zOrder);
private:
	std::vector<HWND> topLevelWindows;

	GLFWwindow* CreateGLFW_Window(
		const char* title,
		GLFWmonitor* monitor,
		GLFWwindow* share);

	static std::string GetWindowTitle(HWND hwnd);
};