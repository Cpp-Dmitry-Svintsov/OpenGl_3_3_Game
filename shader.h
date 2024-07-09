#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "common.h"

class AShader
{
public:
	~AShader();
	AShader(const char* vertexFile, const char* fragmentFile);
	AShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile);

	void Activate();
	void Delete();

	GLuint ID;
private:
	void compileErrors(unsigned int shader, const char* type);
};


#endif