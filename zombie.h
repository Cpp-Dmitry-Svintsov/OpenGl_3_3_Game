#ifndef ZOMBIE_CLASS_H
#define ZOMBIE_CLASS_H

#include "common.h"
#include "model_bones.h"
#include "player.h"
#include "zombies_state.h"

class AsScene;

class AZombie
{
public:
	~AZombie();
	AZombie(AModel_Bones* model, glm::vec3 pos, glm::quat rot, glm::vec3 scale);

	void Init(AsPlayer* player, AsScene* scene);

	void Update();

	void Draw(AShader& shader, ACamera& camera);

	void Update_Path();
	glm::vec3 Get_Pos();

	float Radius = 0.8f;

	bool Is_Alive = true;
	std::vector<glm::mat4> Bones_Matrices;
	std::vector<SBones_Node> Bones;

	std::vector<SCoord> Path;

private:
	void Moove();

	AModel_Bones *Model;
	glm::vec3 Pos;
	glm::vec3 Scale;
	glm::quat Rot;

	glm::mat4 Mat4_Model;


	AsPlayer* Player;
	AsScene* Scene;
	AZombie_State Zombie_State;

	glm::vec3 Speed_Ceil{};
	glm::vec3 Target_Ceil_Coord;
	SCoord Dir;
};

#endif // !ZOMBIE_CLASS_H

