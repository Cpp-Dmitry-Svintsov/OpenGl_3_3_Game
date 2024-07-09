#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H


#include "common.h"
#include "model_bones.h"
#include "player_state.h"

class AsScene;

class AsPlayer
{
public:
	~AsPlayer();
	AsPlayer(AModel_Bones *model, glm::vec3 pos, glm::quat rot, glm::vec3 scale);

	void Init(GLFWwindow* window, ACamera *camera, AsScene *scene);

	void Update();
	void Draw(AShader& shader, ACamera& camera);

	void Rotate(glm::vec3 pos, glm::quat rot, int bone_index);
	void Add_Project_Tile();
	glm::vec3 Get_Pos();

	std::vector<glm::mat4> Bones_Matrices;
	std::vector<SBones_Node> Bones;

	float Radius = 0.5f;

private:
	void Key_Pressed();

	float Speed = 0.07f;

	glm::vec3 Pos;
	glm::quat Rot;
	glm::vec3 Scale;

	AModel_Bones*Model;

	glm::mat4 Mat4_Model;

	AsPlayer_State Player_State;

	GLFWwindow* Window;
	ACamera* Camera;
	AsScene* Scene;


	EKey_Pressing Key_State;
	bool Is_Space_Pressed = false;


};

#endif // !PLAYER_CLASS_H

