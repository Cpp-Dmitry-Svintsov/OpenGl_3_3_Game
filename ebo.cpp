#include "ebo.h"

//=================================================================================================
// constructors
//=================================================================================================
AEBO::AEBO(std::vector<GLuint>& indices)
{
	glGenBuffers(1, &Id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

AEBO::AEBO(GLuint* indices, GLsizeiptr size)
{
	glGenBuffers(1, &Id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

//=================================================================================================
void AEBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
}

//=================================================================================================
void AEBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//=================================================================================================
void AEBO::Delete()
{
	glDeleteBuffers(1, &Id);
}

//=================================================================================================