#include "vao.h"

AVAO::AVAO()
{
	glGenVertexArrays(1, &Id);
}
//=================================================================================================
void AVAO::LinkAttrib(AVBO &vbo, GLuint layout, GLuint num_component, GLenum type, GLsizeiptr stride, void* offset)
{
	vbo.Bind();
	glVertexAttribPointer(layout, num_component, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.Unbind();
}
//=================================================================================================
void AVAO::Bind()
{
	glBindVertexArray(Id);
}
//=================================================================================================
void AVAO::Unbind()
{
	glBindVertexArray(0);
}
//=================================================================================================
void AVAO::Delete()
{
	glDeleteVertexArrays(1, &Id);
}
//=================================================================================================
