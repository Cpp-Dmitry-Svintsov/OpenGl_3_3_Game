#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include "common.h"

class AEBO
{
public:
	AEBO(std::vector<GLuint>& indices);
	AEBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();

private:
	GLuint Id;
};

#endif // !VBO_CLASS_H