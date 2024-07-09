#include "zombie.h"

#include "scene.h"

//===========================================================================================================================================
// destructro & construct
//===========================================================================================================================================
AZombie::~AZombie()
{

}

AZombie::AZombie(AModel_Bones* model, glm::vec3 pos, glm::quat rot, glm::vec3 scale)
	:Model(model), Pos(pos), Rot(rot), Scale(scale)
{
	Bones_Matrices = Model->Bones_Matrices;
	Bones = Model->Bones;

	//Model transsformation:
	Mat4_Model = glm::mat4(1.f);

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot1 = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	trans = glm::translate(trans, pos);
	rot1 = glm::mat4_cast(rot);
	sca = glm::scale(sca, scale);

	// Multiply all matrices together
	Mat4_Model = Mat4_Model * trans * rot1 * sca;

	//=================================================
	//correction
	//hadns
	Bones[1].Par_Pos = glm::vec3(-0.5f, 5.5f, 0.f);
	Bones[2].Par_Pos = glm::vec3(-1.5f, 5.5f, 0.f);
	Bones[1].Childrends_Index.push_back(2);

	Bones[3].Par_Pos = glm::vec3(0.5f, 5.5f, 0.f);
	Bones[4].Par_Pos = glm::vec3(1.5f, 5.5f, 0.f);
	Bones[3].Childrends_Index.push_back(4);

	//hands
	Bones[8].Par_Pos = glm::vec3(-0.5f, 3.f, 0.f);
	Bones[9].Par_Pos = glm::vec3(-0.5f, 1.5f, 0.f);
	Bones[8].Childrends_Index.push_back(9);

	Bones[11].Par_Pos = glm::vec3(-0.5f, 3.f, 0.f);
	Bones[12].Par_Pos = glm::vec3(-0.5f, 1.5f, 0.f);
	Bones[11].Childrends_Index.push_back(12);



	//Player_State.Init(this);
	//Player_State = EPlayer_State::Stay;
}

//Initizlization:
//===========================================================================================================================================
void AZombie::Init(AsPlayer* player, AsScene* scene)
{
	Player = player;
	Scene = scene;
	Zombie_State.Init(this, Scene);
	Zombie_State = EZombie_State::Stay;
}


//===========================================================================================================================================
// public section:
//===========================================================================================================================================
void AZombie::Update()
{
	Zombie_State.Update();

	//patrh finder act
	if (Zombie_State.Mooving_Substate == EZombie_Mooving_Substate::None)
	{
		int len = Path.size();
		if (len > 2)
		{
			Zombie_State = EZombie_State::Moove;
			
			Speed_Ceil.x = Path[len - 2].Y - Path[len - 1].Y;
			Speed_Ceil.z = Path[len - 1].X - Path[len - 2].X;

			Speed_Ceil += Mat4_Model[3];
			Speed_Ceil.x;
			Speed_Ceil.z;
			Speed_Ceil.y = Pos.y;

		}
	}

	if (Zombie_State.Mooving_Substate != EZombie_Mooving_Substate::None)
		Moove();
}

//===========================================================================================================================================
void AZombie::Draw(AShader& shader, ACamera& camera)
{
	Model->Draw(shader, camera, Bones_Matrices, Mat4_Model);
}

//===========================================================================================================================================
void AZombie::Update_Path()
{
	if (Zombie_State.Mooving_Substate == EZombie_Mooving_Substate::None)
		Scene->Get_Path(Mat4_Model[3], Scene->Player->Get_Pos(), Path);
}

//===========================================================================================================================================
glm::vec3 AZombie::Get_Pos()
{
	return glm::vec3(Mat4_Model[3]);
}

//===========================================================================================================================================
// private section:
//===========================================================================================================================================
void AZombie::Moove()
{

	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);



	int len = Path.size();
	glm::vec3 speed(0.f);
	//speed = Scene->Ceil_Reversed_Transform(Dir);
	speed.x = Path[len - 2].Y - Path[len - 1].Y;
	speed.z = Path[len - 1].X - Path[len - 2].X;
	float sp = 0.008f;

	Pos += speed * sp;

	//Mat4_Model = glm::translate(Mat4_Model, speed * sp);


	trans = glm::translate(trans, Pos);
	rot = glm::mat4_cast(Rot);
	sca = glm::scale(sca, Scale);

	// Multiply all matrices together
	Mat4_Model = glm::mat4(1.f) * trans * rot * sca;



	glm::quat rot1;
	if (speed.x == -1.f && speed.z == 0.f)
		Rot = glm::quat(cos(glm::radians(-90.0f / 2)), 0, sin(glm::radians(-90.0f / 2)) * 1, 0);

	else if (speed.x == 0.f && speed.z == -1.f)
		Rot = glm::quat(cos(glm::radians(90.0f)), 0, sin(glm::radians(90.0f)) * 1, 0);

	else if (speed.x == 1.f && speed.z == 0.f)
		Rot = glm::quat(cos(glm::radians(90.0f / 2)), 0, sin(glm::radians(90.0f / 2)) * 1, 0);

	else if (speed.x == 0.f && speed.z == 1.f)
		Rot = glm::quat(0, 0, 0, 0);







	if (AsSettings::Distance_Beetween_Two_Points(Speed_Ceil, Mat4_Model[3]) < 0.02f)
	{
		//Model transsformation:
		Mat4_Model = glm::mat4(1.f);

		// Initialize matrices
		trans = glm::mat4(1.0f);
		rot = glm::mat4(1.0f);
		sca = glm::mat4(1.0f);

		// Use translation, rotation, and scale to change the initialized matrices
		trans = glm::translate(trans, Speed_Ceil);
		rot = glm::mat4_cast(rot1);
		sca = glm::scale(sca, glm::vec3(0.27f));

		// Multiply all matrices together
		Mat4_Model = Mat4_Model * trans * rot * sca;
		


		Zombie_State = EZombie_State::Stay;
	}

}

//===========================================================================================================================================

