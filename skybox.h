#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"
#include "settings.h"

class ASky_Box
{
public:
	ASky_Box( const char* path, const char* file_extension );

	void Draw(AShader& shader, ACamera& camera);

private:
	unsigned int Skybox_Vao;
	unsigned int Cubemap_Texture;
};

#endif // !SKYBOX_CLASS_H
