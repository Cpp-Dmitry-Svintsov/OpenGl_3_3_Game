#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "common.h"

//=================================================================================================
class AVBO
{
public:
	AVBO(std::vector<SVertex>& vertices);
	AVBO(std::vector<SVertex_Bones>& vertices);
	AVBO(std::vector<glm::mat4>& mat4s);
	AVBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();

private:
	GLuint Id;
};

#endif // !VBO_CLASS_H

