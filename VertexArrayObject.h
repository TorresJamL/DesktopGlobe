#ifndef VertexArrayObject_Class_H
#define VertexArrayObject_Class_H

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "VertexBufferObject.h"

class VertexArrayObject {
public:
	GLuint ID;
	VertexArrayObject();

	void LinkVertexBufferObject(VertexBufferObject VBO, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();
};


#endif // !VertexArrayObject_Class_H

