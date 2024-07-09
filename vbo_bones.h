#ifndef VBO_BONES_CLASS_H
#define VBO_BONES_CLASS_H

#include "common.h"

//=================================================================================================
class AVbo_Bones
{
public:
	AVbo_Bones(std::vector<SVertex_Bones>& vertices);

	void Bind();
	void Unbind();
	void Delete();

private:
	GLuint Id;
};

#endif // !VBO_BONES_CLASS_H
