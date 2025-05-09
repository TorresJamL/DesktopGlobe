#include <GL/glew.h>
#include "VertexArrayObject.h"
#include "GLFW/glfw3.h"
#include "VertexBufferObject.h"

VertexArrayObject::VertexArrayObject() {
	glGenVertexArrays(1, &ID);
}
void VertexArrayObject::LinkAttrib(
		VertexBufferObject VBO, 
		GLuint layout, 
		GLuint numComponents, 
		GLenum type, 
		GLsizeiptr stride, 
		void* offset) {
	VBO.Bind();
	// Configure the Vertex Attribute so that openGl knows how to read the VBO
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	// Enable the attribute so openGL kows how to use it
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}
void VertexArrayObject::Bind() {
	glBindVertexArray(ID);
}
void VertexArrayObject::Unbind() {
	glBindVertexArray(0);
}
void VertexArrayObject::Delete() {
	glDeleteVertexArrays(1, &ID);
}