#include "Sphere.h"
#include <Windows.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <string>      // for std::string
#include <sstream>     // for std::ostringstream
#include <vector>

#include "shaderClass.h"
#include "Config.h"

void static DebugVec3(const std::string& label, const glm::vec3& v) {
	std::ostringstream oss;
	oss << label << ": (" << v.x << ", " << v.y << ", " << v.z << ")\n";
	OutputDebugStringA(oss.str().c_str());
}

Sphere::~Sphere() {}
Sphere::Sphere(
	float radius, 
	int sectorCount, 
	int stackCount,
	int screenWidth,
	int screenHeight,
	float fovDeg,
	float camInit_Z,
	float distanceFromCamera)
{
	this->radius = radius;
	this->sectorCount = sectorCount;
	this->stackCount = stackCount;
	this->translation = getCornerTranslation(screenWidth, screenHeight, camInit_Z, fovDeg, distanceFromCamera, true);
	this->vertices = generateSphereVertices(radius, sectorCount, stackCount);
	this->indices = generateSphereIndices(sectorCount, stackCount);
}
Sphere::Sphere(int screenWidth, int screenHeight) {
	radius = Config::globeRadius;
	sectorCount = Config::globeSectorCount;
	stackCount = Config::globeStackCount;
	this->translation = getCornerTranslation(screenWidth, screenHeight, Config::camDist, Config::fovDeg, Config::camDist, true);
	this->vertices = generateSphereVertices(radius, sectorCount, stackCount);
	this->indices = generateSphereIndices(sectorCount, stackCount);
}

std::vector<GLfloat> Sphere::getVertices() {
	return vertices;
}

std::vector<GLuint> Sphere::getIndices() {
	return indices;
}

void Sphere::Draw(
	Shader shader,
	float orientationAngle,
	float rotationAngle,
	glm::vec3 orientationUnitVect,
	glm::vec3 rotationUnitVect)
{
	glm::mat4 model = glm::mat4(1.0f);

	// Apply fixed orientation
	model = glm::rotate(model, glm::radians(orientationAngle), orientationUnitVect);

	// Apply idle rotation 
	model = glm::rotate(model, glm::radians(rotationAngle), rotationUnitVect);

	// Apply the corner translation to the model.
	model = translation * model;

	GLuint modelLoc = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

glm::mat4 Sphere::getCornerTranslation(
	int width, int height,
	float camZ,
	float fov,
	float distanceFromCamera,
	bool bottomRight)
{
	float aspect = (float)width / (float)height;

	float halfHeight = tan(glm::radians(fov / 2.0f)) * distanceFromCamera;
	float halfWidth = halfHeight * aspect;
	
	float xOffset = halfWidth - ( 0.15f + radius);
	float yOffset = halfHeight - ( 0.05f + radius);

	float x = bottomRight ? +xOffset : -xOffset;
	float y = bottomRight ? -yOffset : +yOffset;
	float z = camZ - distanceFromCamera;

	glm::vec3 translation = glm::vec3(x, y, z);
	return glm::translate(glm::mat4(1.0f), translation);
}

void Sphere::Inputs(
	GLFWwindow* window, 
	float deltatime, 
	int width, int height, 
	glm::vec3 camPos,
	glm::mat4 camViewMat4 , 
	glm::mat4 camProjMat4) 
{
	if (!isInteractable) return;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		isInteracting = true;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		float x, y;

		glm::vec3 spherePos = glm::vec3(translation[3]);

		glm::vec3 screenPos = getSphereScreenVector(width, height, camPos, camViewMat4, camProjMat4);
		x = screenPos.x;
		y = height - screenPos.y;

		if (firstClick) {
			glfwSetCursorPos(window, x, y);
			firstClick = false;
		}
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
		isInteracting = false;
	} else {
		isInteracting = false;
	}
}

glm::vec3 Sphere::getSphereScreenVector(int width, int height, glm::vec3 camPos, glm::mat4 camViewMat4, glm::mat4 camProjMat4) {
	glm::vec3 spherePos = glm::vec3(translation[3]);

	glm::vec3 screenPos = glm::project(
		spherePos,
		camViewMat4,
		camProjMat4,
		glm::vec4(0, 0, width, height));
	return screenPos;
}

void Sphere::Rotate(
	GLFWwindow* window, 
	float deltatime, 
	int width, int height, 
	glm::vec3 camPos, 
	glm::mat4 camViewMat4, 
	glm::mat4 camProjMat4) 
{
	if (!isInteracting) {
		rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f); 
		orientationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
		orientation = initOrientation;
		rotation += initRotation * deltatime;
		if (rotation > 360.0f)
			rotation -= 360.0f;
	}
	else if (isInteracting) {
		double mouseX, mouseY;
		glm::vec3 sphereScreenPos = getSphereScreenVector(width, height, camPos, camViewMat4, camProjMat4);

		glfwGetCursorPos(window, &mouseX, &mouseY);
		float x = sphereScreenPos.x;
		float y = height - sphereScreenPos.y;

		float rotx = (mouseX - x) / (2 * radius);
		float roty = (mouseY - y) / (2 * radius);
		
		if (abs(rotx) < 2 && abs(rotx) > 1) {
			rotation += 0;
		} else {
			rotation += sensitivity * -rotx * deltatime;
		}
		if (abs(roty) < 2 && abs(roty) > 1) {
			orientation += 0;
		}
		else {
			orientation += sensitivity * -roty * deltatime;
		}

		glfwSetCursorPos(window, x, y);
	}
}

std::vector<GLfloat> Sphere::generateSphereVertices(float radius, unsigned int sectorCount, unsigned int stackCount) {
	std::vector<GLfloat> vertices;

	for (unsigned int i = 0; i <= stackCount; ++i) {
		float stackAngle = (glm::pi<float>() / 2) - (i * (glm::pi<float>() / stackCount)); // from pi/2 to -pi/2
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for (unsigned int j = 0; j <= sectorCount; ++j) {
			float sectorAngle = j * 2 * glm::pi<float>() / sectorCount;

			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);

			// main vertices
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// Add colors
			vertices.push_back(1.0f); // R
			vertices.push_back(1.0f); // G
			vertices.push_back(1.0f); // B

			// Add texture coordinates
			vertices.push_back(((float)j) / sectorCount); // u
			vertices.push_back(((float)i) / stackCount);  // v
		}
	}
	return vertices;
}

std::vector<GLuint> Sphere::generateSphereIndices(unsigned int sectorCount, unsigned int stackCount) {
	std::vector<GLuint> indices;

	for (unsigned int i = 0; i < stackCount; ++i) {
		for (unsigned int j = 0; j < sectorCount; ++j) {
			unsigned int first = i * (sectorCount + 1) + j;
			unsigned int second = first + sectorCount + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}
	return indices;
}
