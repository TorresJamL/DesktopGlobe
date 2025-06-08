#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
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
	bool isInteractable = true;
	bool firstClick = true;
	bool isInteracting = false;
	float initRotation = 20.0f;
	float initOrientation = -90.0f;
	float rotation = 0.0f;
	float orientation = -90.0f; // rotation to fix initial orientation
	float sensitivity = 10.0f;
	glm::vec3 orientationAxis = glm::vec3(1.0f, 0.0f, 0.0f); // x-axis
	glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f); // z-axis

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
	void Inputs(GLFWwindow* window, float deltatime, int width, int height, glm::vec3 camPos, glm::mat4 camViewMat4, glm::mat4 camProjMat4);
	glm::vec3 getSphereScreenVector(
		int width, int height, 
		glm::vec3 camPos, 
		glm::mat4 camViewMat4,
		glm::mat4 camProjMat4);
	void Rotate(GLFWwindow* window, float deltatime, int width, int height, glm::vec3 camPos, glm::mat4 camViewMat4, glm::mat4 camProjMat4);
private:
	// Create the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 translation;
	glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

	int sectorCount;
	int stackCount;
	bool isDragging = false;
	float radius; 
	float initTime = 0.0f;
	const float HELD_THRESHOLD = 3.0f;

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
