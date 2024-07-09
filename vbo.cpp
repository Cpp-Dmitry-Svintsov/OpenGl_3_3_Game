#include "vbo.h"

//=================================================================================================
// constructors
//=================================================================================================
AVBO::AVBO(std::vector<SVertex>& vertices)
{
	glGenBuffers(1, &Id);
	glBindBuffer(GL_ARRAY_BUFFER, Id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SVertex), vertices.data(), GL_STATIC_DRAW);
}

AVBO::AVBO(std::vector<SVertex_Bones>& vertices)
{
	glGenBuffers(1, &Id);
	glBindBuffer(GL_ARRAY_BUFFER, Id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SVertex_Bones), vertices.data(), GL_STATIC_DRAW);
}


AVBO::AVBO(std::vector<glm::mat4>& mat4s)
{
	glGenBuffers(1, &Id);
	glBindBuffer(GL_ARRAY_BUFFER, Id);
	glBufferData(GL_ARRAY_BUFFER, mat4s.size() * sizeof(glm::mat4), mat4s.data(), GL_STATIC_DRAW);
}

AVBO::AVBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &Id);
	glBindBuffer(GL_ARRAY_BUFFER, Id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

//=================================================================================================
// public section:
//=================================================================================================
void AVBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, Id);
}

//=================================================================================================
void AVBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//=================================================================================================
void AVBO::Delete()
{
	glDeleteBuffers(1, &Id);
}

//=================================================================================================
