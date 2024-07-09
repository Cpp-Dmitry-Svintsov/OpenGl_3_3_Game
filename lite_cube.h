#ifndef LITE_CUBE_CLASS_H
#define LITE_CUBE_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

class ALite_Cube
{
public:
	~ALite_Cube();
	ALite_Cube(glm::vec3 pos, glm::vec3 color, glm::vec3 scale = glm::vec3(1.f), glm::mat4 model = glm::mat4(1.f) );

	void Draw(AShader& shader_program, ACamera& camera);

	glm::vec3 Pos;
	glm::vec4 Color;
	glm::mat4 Model;
private:
	int Count_Of_Indices;


	AVAO Vao;
};

#endif // !LITE_CUBE_CLASS_H

