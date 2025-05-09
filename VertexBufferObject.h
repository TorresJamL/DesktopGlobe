#ifndef VertexBufferObject_Class_H
#define VertexBufferObject_Class_H

#include <GL/glew.h>
#include "GLFW/glfw3.h"

class VertexBufferObject {
public:
	GLuint ID;
	VertexBufferObject(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif // !VertexBufferObject_Class_H
