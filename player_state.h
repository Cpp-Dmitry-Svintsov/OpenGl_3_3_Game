#ifndef PLAYER_STATE_CLASS_H
#define PLAYER_STATE_CLASS_H

#include "common.h"

enum class EPlayer_State : unsigned char
{
	Stay,
	Moove,
	Attack,
	End_Attack,
	Dead,
};

//==================================================================================================================================================================
enum class EPlayer_Attack_Substate : unsigned char
{
	None,
	Stage1,
	Stage2,
};

//==================================================================================================================================================================
enum class EPlayer_Mooving_Substate : unsigned char
{
	None,
	Hands_And_Foots,
	Only_Foots,
};


class AsPlayer;
//==================================================================================================================================================================
class AsPlayer_State
{
public:
	~AsPlayer_State();
	AsPlayer_State();

	void Init(AsPlayer *player);

	operator EPlayer_State() const;
	void operator=(EPlayer_State new_state);

	void Update();

private:
	void Set_Attack_State_Pose();
	void Reset_Hands();
	void Reset_Foots();
	void Mooving_Hadns();
	void Mooving_State();
	void Set_Mooving_Pos();
	void Attack_Act();

	EPlayer_Attack_Substate Attack_Substate = EPlayer_Attack_Substate::None;
	EPlayer_Mooving_Substate Mooving_Substate = EPlayer_Mooving_Substate::None;


	EPlayer_State Player_State;

	float Anim_Speed = 3.f;
	float Anim_Timer;
	float Is_Revers;

	float Attack_Timer = 0.f;

	AsPlayer* Player;
};

#endif // !PLAYER_STATE_CLASS_H

