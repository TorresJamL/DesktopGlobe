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

class GlobeWindow {
public:
	const char* title;
	GLFWwindow* GLFWwnd;
	HWND hwnd;
	int width, height;

	~GlobeWindow();
	GlobeWindow();
	GlobeWindow(const char* title);
	void ShouldClose();
	void updateMousePassThrough();
	void SetWindowStyles();
private:
	std::vector<HWND> topLevelWindows;

	GLFWwindow* CreateGLFW_Window(
		const char* title,
		GLFWmonitor* monitor,
		GLFWwindow* share);

	static std::string GetWindowTitle(HWND hwnd);
};