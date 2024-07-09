#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include "vbo.h"

class AVAO
{
public:
	AVAO();

	void LinkAttrib(AVBO &vbo, GLuint layout, GLuint num_component, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();

private:
	GLuint Id;

};

#endif