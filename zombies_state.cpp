#include "zombie.h"
#include "zombies_state.h"

//============================================================================================================================================
// destructor & constructor && Init && Operator Redifinition
//============================================================================================================================================
AZombie_State::~AZombie_State()
{

}

AZombie_State::AZombie_State()
{

}

//Init
void AZombie_State::Init(AZombie* zombie, AsScene* scene)
{
	Zombie = zombie;
	Scene = scene;
}


void AZombie_State::operator=(EZombie_State new_state)
{
	switch (new_state)
	{
	case EZombie_State::Stay:
		Reset_Foots();
		Reset_Hands();
		Mooving_Substate = EZombie_Mooving_Substate::None;
		break;
	case EZombie_State::Moove:
		Set_Mooving_Pos();
		Moove_Timer = 0.f;
		Is_Revers = 1.f;
		break;
	case EZombie_State::Attack:
		Set_Attack_State_Pose();
		Attack_Timer = 0.f;
		Is_Attack_Revers = 1.f;
		break;
	case EZombie_State::End_Attack:
		break;
	case EZombie_State::Dead:
		break;
	default:
		break;
	}
	Zombie_State = new_state;
}
//============================================================================================================================================
// public section:
//============================================================================================================================================
void AZombie_State::Update()
{
	if (Mooving_Substate != EZombie_Mooving_Substate::None)
		Mooving_State();

	if (Attack_Substate != EZombie_Attack_Substate::None)
		Attack_Act();
}

//============================================================================================================================================
// private section:
//============================================================================================================================================
void AZombie_State::Rotate_Bone(glm::vec3 pos, glm::quat rot, int bone_id)
{

	Zombie->Bones_Matrices[bone_id] = glm::translate(Zombie->Bones_Matrices[bone_id], pos);
	Zombie->Bones_Matrices[bone_id] = glm::rotate(Zombie->Bones_Matrices[bone_id],
		glm::radians(rot.w), glm::vec3(rot.x, rot.y, rot.z));
	Zombie->Bones_Matrices[bone_id] = glm::translate(Zombie->Bones_Matrices[bone_id], -pos);

	if (Zombie->Bones[bone_id].Childrends_Index.size() > 0)
		for (int i{}; i < Zombie->Bones[bone_id].Childrends_Index.size(); ++i)
		{
			int child_index = Zombie->Bones[bone_id].Childrends_Index[i];
			Rotate_Bone(pos, rot, child_index);
		}

}

//============================================================================================================================================
void AZombie_State::Set_Attack_State_Pose()
{
	Reset_Hands();

	//hands
	int index = 3;
	glm::quat rot2 = glm::quat(-65.f, 0.f, 1.f, 0.f);
	Rotate_Bone(Zombie->Bones[index].Par_Pos, rot2, index);

	index = 4;
	rot2 = glm::quat(-35.f, 0.0f, 1.f, 0.f);
	Rotate_Bone(Zombie->Bones[index].Par_Pos, rot2, index);

	index = 1;
	rot2 = glm::quat(65.f, 0.0f, 1.f, 0.f);
	Rotate_Bone(Zombie->Bones[index].Par_Pos, rot2, index);

	index = 2;
	rot2 = glm::quat(75.f, 0.0f, 1.f, 0.f);
	Rotate_Bone(Zombie->Bones[index].Par_Pos, rot2, index);


	if (Mooving_Substate != EZombie_Mooving_Substate::None)
		Mooving_Substate = EZombie_Mooving_Substate::Only_Foots;

	Attack_Substate = EZombie_Attack_Substate::Stage1;

	Attack_Timer = 0.f;
}

//============================================================================================================================================
void AZombie_State::Reset_Hands()
{
	Zombie->Bones_Matrices[1] = glm::mat4(1.f);
	Zombie->Bones_Matrices[2] = glm::mat4(1.f);

	Zombie->Bones_Matrices[3] = glm::mat4(1.f);
	Zombie->Bones_Matrices[4] = glm::mat4(1.f);

	glm::quat rot3 = glm::quat(70.f, 0.f, 0.f, 1.f);
	glm::quat rot4 = glm::quat(-70.f, 0.f, 0.f, 1.f);
	Rotate_Bone(Zombie->Bones[1].Par_Pos, rot3, 1);
	Rotate_Bone(Zombie->Bones[3].Par_Pos, rot4, 3);
}
void AZombie_State::Reset_Foots()
{
	//foots
	Zombie->Bones_Matrices[11] = glm::mat4(1.f);
	Zombie->Bones_Matrices[12] = glm::mat4(1.f);

	Zombie->Bones_Matrices[8] = glm::mat4(1.f);
	Zombie->Bones_Matrices[9] = glm::mat4(1.f);
}
void AZombie_State::Mooving_Hadns()
{
	if (Mooving_Substate != EZombie_Mooving_Substate::None)
		Mooving_State();
}
void AZombie_State::Mooving_State()
{
	Moove_Timer += AsSettings::Delta_Time;

	glm::quat rot = glm::quat(0.5f * Is_Revers, 1.f, 0.f, 0.f);
	glm::quat rot2 = glm::quat(-0.5f * Is_Revers, 1.f, 0.f, 0.f);


	//moove foots
	if (Mooving_Substate == EZombie_Mooving_Substate::Hands_And_Foots)
	{
		rot = glm::quat(-0.35f * Is_Revers, 0.f, 1.f, 0.f);
		rot2 = glm::quat(-0.35f * Is_Revers, 0.f, 1.f, 0.f);
		Rotate_Bone(Zombie->Bones[1].Par_Pos, rot, 1);
		Rotate_Bone(Zombie->Bones[3].Par_Pos, rot2, 3);
	}
	else if (Attack_Substate == EZombie_Attack_Substate::None)
	{
		Set_Mooving_Pos();
		Mooving_Substate = EZombie_Mooving_Substate::Hands_And_Foots;
	}

	rot = glm::quat(0.35f * Is_Revers, 1.f, 0.f, 0.f);
	rot2 = glm::quat(-0.35f * Is_Revers, 1.f, 0.f, 0.f);

	Rotate_Bone(Zombie->Bones[11].Par_Pos, rot, 11);
	Rotate_Bone(Zombie->Bones[8].Par_Pos, rot2, 8);

	if (Moove_Timer > 0.5f)
	{
		Moove_Timer = 0.f;
		Is_Revers *= -1.f;
	}

}
void AZombie_State::Set_Mooving_Pos()
{
	if (Mooving_Substate == EZombie_Mooving_Substate::None)
	{
		glm::quat rot = glm::quat(-10.f, 1.f, 0.f, 0.f);
		glm::quat rot2 = glm::quat(10.f, 1.f, 0.f, 0.f);
		Rotate_Bone(Zombie->Bones[11].Par_Pos, rot, 11);
		Rotate_Bone(Zombie->Bones[8].Par_Pos, rot2, 8);
	}



	glm::quat rot3 = glm::quat(10.f, 0.f, 1.f, 0.0f);
	glm::quat rot4 = glm::quat(10.f, 0.f, 1.f, 0.0f);
	Rotate_Bone(Zombie->Bones[1].Par_Pos, rot3, 1);
	Rotate_Bone(Zombie->Bones[3].Par_Pos, rot4, 3);



	if (Attack_Substate == EZombie_Attack_Substate::None)
		Mooving_Substate = EZombie_Mooving_Substate::Hands_And_Foots;
	else
		Mooving_Substate = EZombie_Mooving_Substate::Only_Foots;
}
void AZombie_State::Attack_Act()
{
	Attack_Timer += AsSettings::Delta_Time;

	int index;
	glm::quat rot2;

	//moove hands 1
	index = 2;
	rot2 = glm::quat(-0.25f * Is_Attack_Revers, 0.f, 1.f, 0.f);
	Rotate_Bone(Zombie->Bones[index].Par_Pos, rot2, index);

	index = 4;
	rot2 = glm::quat(-0.25f * Is_Attack_Revers, 0.f, 1.f, 0.f);
	Rotate_Bone(Zombie->Bones[index].Par_Pos, rot2, index);




	if (Attack_Timer > 1.f)
	{
		Attack_Timer = 0.f;
		Is_Attack_Revers *= -1.f;
	}
}