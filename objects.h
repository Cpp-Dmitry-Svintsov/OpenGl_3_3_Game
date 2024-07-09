#ifndef OBJECTS_CLASS_H
#define OBJECTS_CLASS_H

#include "common.h"
#include "lite_cube.h"

class AProject_Tile
{
public:
	~AProject_Tile();
	AProject_Tile(glm::mat4 model, glm::vec3 scale = glm::vec3(1.f), glm::vec3 color = glm::vec3(0.f));

	void Update();
	void Draw(AShader& shader, ACamera& camera);

	glm::vec3 Get_Pos();

	bool Is_Alive = true;

private:

	float Speed = 0.3f;
	float Timer{};

	ALite_Cube *Tile;







};

#endif // !OBJECTS_CLASS_H

