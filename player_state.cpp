#include "player_state.h"
#include "player.h"

//================================================================================================================================================================
// destructor & constructor
//================================================================================================================================================================
AsPlayer_State::~AsPlayer_State()
{}

AsPlayer_State::AsPlayer_State()
{}

//---------------------------------------
//init
void AsPlayer_State::Init(AsPlayer *player)
{
	Player = player;
}

//--------------------------------------
//operator redifinition
AsPlayer_State::operator EPlayer_State() const
{
	return Player_State;
}
void AsPlayer_State::operator=(EPlayer_State new_state)
{
	switch (new_state)
	{
	case EPlayer_State::Stay:
		Reset_Hands();
		Reset_Foots();
		Mooving_Substate = EPlayer_Mooving_Substate::None;

		break;
	case EPlayer_State::Moove:
		Set_Mooving_Pos();
		Anim_Timer = 0.f;
		Is_Revers = 1.f;
		break;
	case EPlayer_State::Attack:
		Set_Attack_State_Pose();
		break;
	case EPlayer_State::End_Attack:
		Attack_Substate = EPlayer_Attack_Substate::None;
		Reset_Hands();
		break;
	case EPlayer_State::Dead:
		break;
	default:
		break;
	}
	Player_State = new_state;
}

//================================================================================================================================================================
// public section:
//================================================================================================================================================================
void AsPlayer_State::Update()
{
	if (Mooving_Substate != EPlayer_Mooving_Substate::None)
		Mooving_State();

	if (Attack_Substate != EPlayer_Attack_Substate::None)
		Attack_Act();
}

//================================================================================================================================================================
// private section:
//================================================================================================================================================================
void AsPlayer_State::Set_Attack_State_Pose()
{
	Reset_Hands();

	//hands
	int index = 3;
	glm::quat rot2 = glm::quat(-57.f, 1.4f, 3.f, 1.f);

	Player->Rotate(Player->Bones[index].Par_Pos, rot2, index);

	index = 4;
	rot2 = glm::quat(-45.f, -2.0f, 7.5f, 2.f);

	Player->Rotate(Player->Bones[index].Par_Pos, rot2, index);

	index = 1;
	rot2 = glm::quat(90.f, 0.0f, 0.4f, 0.f);

	Player->Rotate(Player->Bones[index].Par_Pos, rot2, index);

	index = 2;
	rot2 = glm::quat(45.f, 0.f, 1.f, 2.f);

	Player->Rotate(Player->Bones[index].Par_Pos, rot2, index);

	if (Mooving_Substate != EPlayer_Mooving_Substate::None)
		Mooving_Substate = EPlayer_Mooving_Substate::Only_Foots;

	Attack_Substate = EPlayer_Attack_Substate::Stage1;

	Attack_Timer = 0.f;
}

//================================================================================================================================================================
void AsPlayer_State::Reset_Hands()
{
	Player->Bones_Matrices[1] = glm::mat4(1.f);
	Player->Bones_Matrices[2] = glm::mat4(1.f);

	Player->Bones_Matrices[3] = glm::mat4(1.f);
	Player->Bones_Matrices[4] = glm::mat4(1.f);

	glm::quat rot3 = glm::quat(70.f, 0.f, 0.f, 1.f);
	glm::quat rot4 = glm::quat(-70.f, 0.f, 0.f, 1.f);
	Player->Rotate(Player->Bones[1].Par_Pos, rot3, 1);
	Player->Rotate(Player->Bones[3].Par_Pos, rot4, 3);
}

//================================================================================================================================================================
void AsPlayer_State::Reset_Foots()
{
	//foots
	Player->Bones_Matrices[11] = glm::mat4(1.f);
	Player->Bones_Matrices[12] = glm::mat4(1.f);

	Player->Bones_Matrices[8] = glm::mat4(1.f);
	Player->Bones_Matrices[9] = glm::mat4(1.f);
}

//================================================================================================================================================================
void AsPlayer_State::Mooving_Hadns()
{

}

//================================================================================================================================================================
void AsPlayer_State::Mooving_State()
{
	Anim_Timer += AsSettings::Delta_Time;

	int index;
	glm::quat rot, rot2;


	//moove foots
	if (Mooving_Substate == EPlayer_Mooving_Substate::Hands_And_Foots)
	{
		rot = glm::quat(-0.50f * Is_Revers, 0.f, 1.f, 0.f);
		rot2 = glm::quat(-0.50f * Is_Revers, 0.f, 1.f, 0.f);
		Player->Rotate(Player->Bones[1].Par_Pos, rot, 1);
		Player->Rotate(Player->Bones[3].Par_Pos, rot2, 3);
	}
	else if (Attack_Substate == EPlayer_Attack_Substate::None)
	{
		Set_Mooving_Pos();
		Mooving_Substate = EPlayer_Mooving_Substate::Hands_And_Foots;
	}

	//rot = glm::quat(0.9f * Is_Revers, 1.f, 0.f, 0.f);
	//rot2 = glm::quat(-0.9f * Is_Revers, 1.f, 0.f, 0.f);

	index = 11;
	rot = glm::quat(0.7f * Is_Revers, 1.f, 0.f, 0.f);
	Player->Rotate(Player->Bones[index].Par_Pos, rot, index);

	index = 8;
	rot = glm::quat(-0.7f * Is_Revers, 1.f, 0.f, 0.f);
	Player->Rotate(Player->Bones[index].Par_Pos, rot, index);

	//Player->Rotate(Player->Bones[11].Par_Pos, rot, 11);
	//Player->Rotate(Player->Bones[8].Par_Pos, rot2, 8);

	if (Anim_Timer > 0.7f)
	{
		Anim_Timer = 0.f;
		Is_Revers *= -1.f;
	}
}

//================================================================================================================================================================
void AsPlayer_State::Set_Mooving_Pos()
{

	if (Mooving_Substate == EPlayer_Mooving_Substate::None)
	{
		glm::quat rot = glm::quat(-40.f, 1.f, 0.f, 0.f);
		glm::quat rot2 = glm::quat(40.f, 1.f, 0.f, 0.f);
		Player->Rotate(Player->Bones[11].Par_Pos, rot, 11);
		Player->Rotate(Player->Bones[8].Par_Pos, rot2, 8);
	}



		glm::quat rot3 = glm::quat(30.f, 0.f, 1.f, 0.0f);
		glm::quat rot4 = glm::quat(30.f, 0.f, 1.f, 0.0f);
		Player->Rotate(Player->Bones[1].Par_Pos, rot3, 1);
		Player->Rotate(Player->Bones[3].Par_Pos, rot4, 3);



	if (Attack_Substate == EPlayer_Attack_Substate::None)
		Mooving_Substate = EPlayer_Mooving_Substate::Hands_And_Foots;
	else
		Mooving_Substate = EPlayer_Mooving_Substate::Only_Foots;
}


//================================================================================================================================================================
void AsPlayer_State::Attack_Act()
{
	Attack_Timer += AsSettings::Delta_Time;
	if (Attack_Timer > 0.3f)
	{
		Player->Add_Project_Tile();
		Attack_Timer = 0.f;
	}
}

//================================================================================================================================================================
