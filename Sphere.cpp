#include "Sphere.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vector>
#include "shaderClass.h"

Sphere::Sphere(float radius, int sectorCount, int stackCount) {
	this->radius = radius;
	this->sectorCount = sectorCount;
	this->stackCount = stackCount;
	vertices = generateSphereVertices(radius, sectorCount, stackCount);
	indices = generateSphereIndices(sectorCount, stackCount);
}
Sphere::~Sphere() {

}

std::vector<GLfloat> Sphere::getVertices() {
	return vertices;
}
std::vector<GLuint> Sphere::getIndices() {
	return indices;
}

void Sphere::setSectorCount(int sectorCount) {
	this->sectorCount = sectorCount;
}
void Sphere::setStackCount(int stackCount) {
	this->stackCount = stackCount;
}
void Sphere::setRadius(float radius) {
	this->radius = radius;
}

void Sphere::Update() {
	vertices = generateSphereVertices(radius, sectorCount, stackCount);
	indices = generateSphereIndices(sectorCount, stackCount);
}

void Sphere::Draw(
	Shader shader, 
	float orientationAngle, 
	float rotationAngle, 
	glm::vec3 orientationUnitVect, 
	glm::vec3 rotationUnitVect) 
{
	model = glm::mat4(1.0f);

	// Rotates the sphere 90 degrees down so the americas are facing the camera by default.
	glm::mat4 orientation = glm::rotate(model, glm::radians(orientationAngle), orientationUnitVect);

	// Controls the sphere's rotation around a given axis aka rotationUnitVect. 
	glm::mat4 rotation = glm::rotate(model, glm::radians(rotationAngle), rotationUnitVect);

	model = orientation * rotation;
	// Pass the model matrix to the shader
	GLuint modelLoc = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

}

std::vector<GLfloat> Sphere::generateSphereVertices(float radius, unsigned int sectorCount, unsigned int stackCount) {
	std::vector<GLfloat> vertices;

	for (unsigned int i = 0; i <= stackCount; ++i) {
		float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stackCount; // from pi/2 to -pi/2
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for (unsigned int j = 0; j <= sectorCount; ++j) {
			float sectorAngle = j * 2 * glm::pi<float>() / sectorCount;

			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// Add colors (example: white)
			vertices.push_back(1.0f); // R
			vertices.push_back(1.0f); // G
			vertices.push_back(1.0f); // B

			// Add texture coordinates (optional)
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