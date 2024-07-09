#ifndef PLANE_CLASS_H
#define PLANE_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"
#include "lite_cube.h"

class APlane
{
public:
	~APlane();
	APlane(glm::vec3 pos, float width, float height);

	void Draw(AShader& shader, ACamera& camera);

	void Add_Light_Object(ALite_Cube* obj);

private:
	AVAO Vao;

	glm::vec3 Pos;
	glm::mat4 Model;

	std::vector<ALite_Cube*> Lights;
};

#endif // !PLANE_CLASS_H

