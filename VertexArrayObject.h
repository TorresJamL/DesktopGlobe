#ifndef VertexArrayObject_Class_H
#define VertexArrayObject_Class_H

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "VertexBufferObject.h"

class VertexArrayObject {
public:
	GLuint ID;
	VertexArrayObject();

	void LinkAttrib(VertexBufferObject VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};


#endif // !VertexArrayObject_Class_H

