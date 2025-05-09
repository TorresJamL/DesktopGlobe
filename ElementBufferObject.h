#ifndef ElementBufferObject_Class_H
#define ElementBufferObject_Class_H

#include <GL/glew.h>
#include "GLFW/glfw3.h"

class ElementBufferObject {
public:
	GLuint ID;
	ElementBufferObject(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif // !ElementBufferObject_Class_H

