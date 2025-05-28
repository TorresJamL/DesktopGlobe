#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"

#include <vector>
#include "shaderClass.h"

class Sphere {
public:
	Sphere(
		float radius, 
		int sectorCount, 
		int stackCount,
		int screenWidth,
		int screenHeight,
		float fovDeg,
		float camInit_Z,
		float distanceFromCamera);
	
	float naturalRotation = 0.0f;
	bool isInteracting = false;
	float initialOrientationAngle = -90.0f; // rotation to fix initial orientation
	glm::vec3 initialOrientationAxis = glm::vec3(1.0f, 0.0f, 0.0f); // x-axis
	glm::vec3 naturalRotationAxis = glm::vec3(0.0f, 0.0f, 1.0f); // z-axis

	~Sphere();
	std::vector<GLfloat> getVertices();
	std::vector<GLuint> getIndices();
	void Draw(
		Shader shader,
		float orientationAngle,
		float rotationAngle,
		glm::vec3 orientationUnitVect,
		glm::vec3 rotationUnitVect);
	void setSectorCount(int sectorCount);
	void setStackCount(int stackCount);
	void setRadius(float radius);
	void Inputs(GLFWwindow* window, float deltatime);

private:
	// Create the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 translation;

	bool isDragging = false;
	int sectorCount;
	int stackCount;
	float radius; 


	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	void Update();

	std::vector<GLfloat> static generateSphereVertices(float radius, unsigned int sectorCount, unsigned int stackCount);
	std::vector<GLuint> static generateSphereIndices(unsigned int sectorCount, unsigned int stackCount);
	glm::mat4 getCornerTranslation(
		int width, int height,
		float camZ,
		float fov,
		float distanceFromCamera,
		bool bottomRight);
};

#endif // !SPHERE_CLASS_H
