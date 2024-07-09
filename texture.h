#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H


#include "shader.h"

class ATexture
{
public:
	~ATexture();
	ATexture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	ATexture(const char* image, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	void Tex_Unit(AShader& shader, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();
	void Delete();

	GLuint ID;
	const char* type;
	GLuint unit;

	GLenum type1;

private:
};
#endif