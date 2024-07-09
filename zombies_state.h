#ifndef ZOMBIES_STATE_CLASS_H
#define ZOMBIES_STATE_CLASS_H

#include "common.h"

enum class EZombie_State : unsigned char
{
	Stay,
	Moove,
	Attack,
	End_Attack,
	Dead,
};

//==================================================================================================================================================================
enum class EZombie_Attack_Substate : unsigned char
{
	None,
	Stage1,
	Stage2,
};

//==================================================================================================================================================================
enum class EZombie_Mooving_Substate : unsigned char
{
	None,
	Hands_And_Foots,
	Only_Foots,
};


class AZombie;
class AsScene;
//==================================================================================================================================================================
class AZombie_State
{
public:
	~AZombie_State();
	AZombie_State();

	void Init(AZombie* zombie, AsScene* scene);

	void operator=(EZombie_State new_state);

	void Update();

	EZombie_Attack_Substate Attack_Substate = EZombie_Attack_Substate::None;
	EZombie_Mooving_Substate Mooving_Substate = EZombie_Mooving_Substate::None;

	bool Need_Path = true;

private:
	void Rotate_Bone(glm::vec3 pos, glm::quat rot, int bone_index);
	void Set_Attack_State_Pose();
	void Reset_Hands();
	void Reset_Foots();
	void Mooving_Hadns();
	void Mooving_State();
	void Set_Mooving_Pos();
	void Attack_Act();


	EZombie_State Zombie_State;

	float Moove_Anim_Speed = 3.f;
	float Moove_Timer;
	float Is_Revers;

	float Attack_Timer = 0.f;
	float Is_Attack_Revers;

	AZombie* Zombie;
	AsScene* Scene;
};

#endif // !ZOMBIES_STATE_CLASS_H
