#include "player.h"
#include "scene.h"

//=========================================================================================================================================
// destructor & constuctors
//=========================================================================================================================================
AsPlayer::~AsPlayer()
{}

AsPlayer::AsPlayer(AModel_Bones* model, glm::vec3 pos, glm::quat rot, glm::vec3 scale)
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
	trans = glm::translate(trans, Pos);
	rot1 = glm::mat4_cast(Rot);
	sca = glm::scale(sca, Scale);

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
	


	Player_State.Init(this);
	Player_State = EPlayer_State::Stay;

}

//init================================
void AsPlayer::Init(GLFWwindow* window, ACamera* camera, AsScene* scene)
{
	Window = window;
	Camera = camera;
	Scene = scene;
}

//=========================================================================================================================================
// public section:
//=========================================================================================================================================
void AsPlayer::Update()
{
	Key_Pressed();

	Player_State.Update();
}

void AsPlayer::Draw(AShader& shader, ACamera& camera)
{
	Model->Draw(shader, camera, Bones_Matrices, Mat4_Model, Pos, Rot, Scale);
}

//=========================================================================================================================================
void AsPlayer::Rotate(glm::vec3 pos, glm::quat rot, int bone_id)
{

	Bones_Matrices[bone_id] = glm::translate(Bones_Matrices[bone_id], pos);
	Bones_Matrices[bone_id] = glm::rotate(Bones_Matrices[bone_id],
		glm::radians(rot.w), glm::vec3(rot.x, rot.y, rot.z));
	Bones_Matrices[bone_id] = glm::translate(Bones_Matrices[bone_id], -pos);

	if (Bones[bone_id].Childrends_Index.size() > 0)
		for (int i{}; i < Bones[bone_id].Childrends_Index.size(); ++i)
		{
			int child_index = Bones[bone_id].Childrends_Index[i];
			Rotate(pos, rot, child_index);
		}

}

//=========================================================================================================================================
// private section:
//=========================================================================================================================================

void AsPlayer::Key_Pressed()
{
	glm::mat4 rot = glm::mat4(1.0f);
	glm::quat rotation;

	glm::vec3 new_pos(0.f);
	
	glm::vec3 prev_pos = Mat4_Model[3];
	glm::vec3 cur_pos(0.f);

	float dt = AsSettings::Delta_Time;


	if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
		if (Key_State == EKey_Pressing::Key_None)
		{
			Key_State = EKey_Pressing::Key_Up;
			Player_State = EPlayer_State::Moove;
		}

	if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_RELEASE)
		if (Key_State == EKey_Pressing::Key_Up)
		{
			Key_State = EKey_Pressing::Key_None;
			Player_State = EPlayer_State::Stay;
		}


	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		if (Key_State == EKey_Pressing::Key_None)
		{
			Key_State = EKey_Pressing::Key_Down;
			Player_State = EPlayer_State::Moove;
		}

	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		if (Key_State == EKey_Pressing::Key_Down)
		{
			Key_State = EKey_Pressing::Key_None;
			Player_State = EPlayer_State::Stay;
		}


	//rotation
	if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		Mat4_Model = glm::translate(Mat4_Model, Pos);
		Mat4_Model = glm::rotate(Mat4_Model,
			glm::radians(-1.f), glm::vec3(0.f, 1.f, 0.f));
		Mat4_Model = glm::translate(Mat4_Model, -Pos);
	}

	if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		Mat4_Model = glm::translate(Mat4_Model, Pos);
		Mat4_Model = glm::rotate(Mat4_Model,
			glm::radians(1.f), glm::vec3(0.f, 1.f, 0.f));
		Mat4_Model = glm::translate(Mat4_Model, -Pos);
	}

	//attack
	if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (Is_Space_Pressed == false)
		{
			Player_State = EPlayer_State::Attack;
			Is_Space_Pressed = true;
			
			Add_Project_Tile();
		}
	}
	else if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		if (Is_Space_Pressed == true)
		{
			Player_State = EPlayer_State::End_Attack;
			Is_Space_Pressed = false;
		}
	}

	glm::vec3 cheked_pos{};
	switch (Key_State)
	{
	case EKey_Pressing::Key_Up:
		new_pos = glm::vec3(0.f, 0.f, Speed);
		cheked_pos = glm::vec3(0.f, 0.f, Speed + Radius);
		break;
	case EKey_Pressing::Key_Down:
		new_pos = glm::vec3(0.f, 0.f, -Speed);
		cheked_pos = glm::vec3(0.f, 0.f, -Speed - Radius);
		break;
	default:
		break;
	}

	//chec collisim width static objects
	glm::mat4 copy = Mat4_Model;
	copy = glm::translate(copy, cheked_pos);
	glm::vec3 checked_pos = glm::vec3(copy[3]);

	if (!Scene->Check_Collision(checked_pos))
	{
		Mat4_Model = glm::translate(Mat4_Model, new_pos);
		cur_pos = Mat4_Model[3];

		if (Key_State != EKey_Pressing::Key_None)
		{
			Camera->Position += (cur_pos - prev_pos);
		}
	}
}

//=========================================================================================================================================
void AsPlayer::Add_Project_Tile()
{
	glm::mat4 model = Mat4_Model;
	model = glm::translate(model, glm::vec3(0.7f, 5.f, 3.5f));
	Scene->ProjectTiles.push_back(	new AProject_Tile(model, glm::vec3(1.f, 1.f, 1.f))	);

	Scene->Play_Sound(ESound_Type::Shoot);
}

//=========================================================================================================================================
glm::vec3 AsPlayer::Get_Pos()
{
	return glm::vec3(Mat4_Model[3]);
}

//=========================================================================================================================================
