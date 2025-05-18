#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

#include "GLFW/glfw3.h"

#include <vector>

class Sphere {
public:
	Sphere(float radius, int sectorCount, int stackCount);

	std::vector<GLfloat> getVertices();
	std::vector<GLuint> getIndices();

	void setSectorCount(int sectorCount);
	void setStackCount(int stackCount);
	void setRadius(float radius);
private:
	int sectorCount;
	int stackCount;
	float radius;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	void Update();

	std::vector<GLfloat> static generateSphereVertices(float radius, unsigned int sectorCount, unsigned int stackCount);
	std::vector<GLuint> static generateSphereIndices(unsigned int sectorCount, unsigned int stackCount);
};

#endif // !SPHERE_CLASS_H
