#include "storage.h"
#include "scene.h"

//========================================================================================
// static variables section:
//========================================================================================




//========================================================================================
// protected section constuctor & destructor & init
//========================================================================================
AsStorage::~AsStorage()
{
	//delete all abjects

	//-----------------------------------------			3D MODELS AND SYBOXES			--------------------------------------------------//
	for (auto* el : Models)
		delete el;
	Models.erase(Models.begin(), Models.end());

	for (auto* el : Sky_Boxes)
		delete el;
	Sky_Boxes.erase(Sky_Boxes.begin(), Sky_Boxes.end());

	delete Zombie_Model;

	delete Player_Model;



	//-----------------------------------------			PRIMITIVES			---------------------------------------------------------------//

	for (auto* el : Planes)
		delete el;
	Planes.erase(Planes.begin(), Planes.end());

	for (auto* el : Boxes)
		delete el;
	Boxes.erase(Boxes.begin(), Boxes.end());

	//-----------------------------------------			TEXTURES			---------------------------------------------------------------//
	for (auto* el : Textures_Dif)
	{
		el->Delete();
		delete el;
	}
	Textures_Dif.erase(Textures_Dif.begin(), Textures_Dif.end());

	for (auto* el : Textures_Spec)
	{
		el->Delete();
		delete el;
	}
	Textures_Spec.erase(Textures_Spec.begin(), Textures_Spec.end());

	for (auto* el : Textures)
	{
		el->Delete();
		delete el;
	}
	Textures.erase(Textures.begin(), Textures.end());

	//-----------------------------------------			LIGHTS			---------------------------------------------------------------//
	for (auto* el : Lights)
		delete el;
	Lights.erase(Lights.begin(), Lights.end());

	//----------------------------------------- SOUNDS ------------------------------------------------------------------


};
AsStorage::AsStorage() {};

//init
//========================================================================================
void AsStorage::Init(AsScene* scene)
{
	//sound
	Sound_Main_Theme = Mix_LoadMUS("sounds/main.mp3");
	Sound_Shoot = Mix_LoadWAV("sounds/shoot.wav");
	Sound_Shoot->volume = 75;
	Sound_Hit = Mix_LoadWAV("sounds/hit.wav");


	Sound_Zombie[0] = Mix_LoadWAV("sounds/zombie1.wav");
	Sound_Zombie[0]->volume = 35;
	Sound_Zombie[1] = Mix_LoadWAV("sounds/zombie2.wav");
	Sound_Zombie[1]->volume = 35;
	Sound_Zombie[2] = Mix_LoadWAV("sounds/zombie3.wav");
	Sound_Zombie[2]->volume = 50;
	Sound_Zombie[3] = Mix_LoadWAV("sounds/zombie4.wav");
	Sound_Zombie[3]->volume = 25;

	//play musik
	Mix_PlayMusic(Sound_Main_Theme, -1);

	//
	Scene = scene;

	//textures
	Light_Cube_Tex = new ATexture("textures/my_textures/tex0.png", "diffuse", 0);
	Textures.push_back(Light_Cube_Tex);
	Mramor_Tex = new ATexture("textures/my_textures/tex2.jpg", "diffuse", 0);
	Textures.push_back(Mramor_Tex);
	Terra_Tex = new ATexture("textures/my_textures/terra_baseColor.png", "diffuse", 0);
	Textures.push_back(Terra_Tex);

	//Skyboxes
	Sky_Boxes.push_back(new ASky_Box("models/SKY_BOX/skybox/", ".jpg"));
	Sky_Boxes.push_back(new ASky_Box("models/SKY_BOX/spacebox/", ".png"));



	//
	///3d models
	Boxes.push_back(new ABOX(glm::vec3(0.f, 0.f, 0.f), glm::vec3(40.f, 1.f, 40.f), glm::vec3(0.5f, 0.5f, 0.5f), Terra_Tex, glm::vec3(1.f)));


	//build temple in the scenter
	Build_Temple();
	Buildings();
	Build_Fances();
	Trees();


	Player_Model = new AModel_Bones("models/GAME/green_beret/scene.gltf", glm::vec3(0.f, 2.5f, 0.f), glm::vec3(1.f),
		glm::quat(1, 0, 0, 0));
	//Models_Bones.push_back(player_model);
	//Player = new AsPlayer(player_model, glm::vec3(0.f, 0.5f, 0.3f), glm::quat(0.f, 0, 0, 0.f), glm::vec3(0.25f));

	Zombie_Model = new AModel_Bones("models/GAME/zombie/scene.gltf", glm::vec3(0.f, 2.5f, 0.f), glm::vec3(1.f),
		glm::quat(1, 0, 0, 0));



}

//========================================================================================
// private:section:
//========================================================================================
void AsStorage::Build_Temple()
{

	std::vector <glm::mat4> instanceMatrix;
	int count = 0;
	GLfloat y_offset{}, x_offset{}, z_offset{};

	//enter
	Boxes.push_back(new ABOX(glm::vec3(4.5f, 0.6f, -2.5f), glm::vec3(1.f, 0.5f, 1.f), glm::vec3(0.5f, 0.5f, 0.5f), Mramor_Tex, glm::vec3(0.2f)));

	{
		++count;
		glm::vec3 tempTranslation = glm::vec3(4.5f, 2.8f, -2.5f);
		glm::quat tempRotation = glm::quat(0, 0, 0, 0);
		glm::vec3 tempScale = glm::vec3(2.f);

		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);

		instanceMatrix.push_back(trans * rot * sca);

	}




	Boxes.push_back(new ABOX(glm::vec3(4.5f, 0.6f, 2.5f), glm::vec3(1.f, 0.5f, 1.f), glm::vec3(0.5f, 0.5f, 0.5f), Mramor_Tex, glm::vec3(0.2f)));

	{
		++count;
		glm::vec3 tempTranslation = glm::vec3(4.5f, 2.8f, 2.5f);
		glm::quat tempRotation = glm::quat(0, 0, 0, 0);
		glm::vec3 tempScale = glm::vec3(2.f);

		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);

		instanceMatrix.push_back(trans * rot * sca);

	}

	y_offset = 7.5f;
	for (int j = 0; j < 2; ++j)
	{
		x_offset = 4.5f;
		for (int i = 0; i < 4; ++i)
		{
			Boxes.push_back(new ABOX(glm::vec3(x_offset, 0.6f, y_offset), glm::vec3(1.f, 0.5f, 1.f), glm::vec3(0.5f, 0.5f, 0.5f), Mramor_Tex, glm::vec3(0.2f)));

			{
				++count;
				glm::vec3 tempTranslation = glm::vec3(x_offset, 2.8f, y_offset);
				glm::quat tempRotation = glm::quat(0, 0, 0, 0);
				glm::vec3 tempScale = glm::vec3(2.f);

				glm::mat4 trans = glm::mat4(1.0f);
				glm::mat4 rot = glm::mat4(1.0f);
				glm::mat4 sca = glm::mat4(1.0f);

				// Transform the matrices to their correct form
				trans = glm::translate(trans, tempTranslation);
				rot = glm::mat4_cast(tempRotation);
				sca = glm::scale(sca, tempScale);

				instanceMatrix.push_back(trans * rot * sca);

			}

			x_offset -= 5.f;
		}
		y_offset -= 15.f;
	}

	//back
	y_offset = 7.5f;
	for (int i = 0; i < 4; ++i)
	{
		Boxes.push_back(new ABOX(glm::vec3(-15.5f, 0.6f, y_offset), glm::vec3(1.f, 0.5f, 1.f), glm::vec3(0.5f, 0.5f, 0.5f), Mramor_Tex, glm::vec3(0.2f)));
		
		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(-15.5f, 2.8f, y_offset);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(2.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);

		}

		y_offset -= 5.f;
	}


	//--------------------------------------------------------------
	//UP

	y_offset = 7.5f;
	for (int j = 0; j < 2; ++j)
	{
		x_offset = 2.f;
		for (int i = 0; i < 4; ++i)
		{
			Boxes.push_back(new ABOX(glm::vec3(x_offset, 5.f, y_offset), glm::vec3(4.8f, 0.5f, 0.5f),
				glm::vec3(0.5f, 0.5f, 0.5f), Mramor_Tex, glm::vec3(1.f, 0.1f, 0.1f)));

			x_offset -= 5.f;
		}
		y_offset -= 15.f;
	}

	x_offset = -15.5f;
	for (int j = 0; j < 2; ++j)
	{
		y_offset = 5.f;
		for (int i = 0; i < 3; ++i)
		{
			Boxes.push_back(new ABOX(glm::vec3(x_offset, 5.f, y_offset), glm::vec3(0.5f, 0.5f, 4.8f),
				glm::vec3(0.5f, 0.5f, 0.5f), Mramor_Tex, glm::vec3(0.1f, 0.1f, 1.f)));

			y_offset -= 5.f;
		}
		x_offset += 20.f;
	}


	Models.push_back(new AModel("models/GAME/column_3/scene.gltf", glm::vec3(5.f, 3.1f, -2.5f), glm::vec3(2.f, 2.f, 2.f),
		glm::quat(glm::radians(0.f), 0.f, 1.f, 0.f), count, instanceMatrix	));

	//------------------------------------------------------------------------------------
	//interjer

	{
		std::vector <glm::mat4> instanceMatrix;
		int count = 0;

		x_offset = -15.f;

		z_offset = 5.f;
		for (int j = 0; j < 3; ++j)
		{
			{
				++count;

				glm::vec3 tempTranslation = glm::vec3(-15.5f, 0.15f, z_offset);
				glm::quat tempRotation = glm::quat(cos(glm::radians(90.0f / 2)), 0, 0, sin(glm::radians(90.0f / 2)) * 1);
				glm::vec3 tempScale = glm::vec3(0.5f);

				glm::mat4 trans = glm::mat4(1.0f);
				glm::mat4 rot = glm::mat4(1.0f);
				glm::mat4 sca = glm::mat4(1.0f);

				// Transform the matrices to their correct form
				trans = glm::translate(trans, tempTranslation);
				rot = glm::mat4_cast(tempRotation);
				sca = glm::scale(sca, tempScale);

				instanceMatrix.push_back(trans * rot * sca);

				x_offset += 6.f;
			}

			z_offset -= 5.f;
		}

		Models.push_back(new AModel("models/GAME/statue_gargulie/scene.gltf", glm::vec3(-15.f, 0.15f, z_offset), glm::vec3(3.f),
			glm::quat(cos(glm::radians(90.0f / 2)), 0, 0, sin(glm::radians(90.0f / 2)) * 1), count, instanceMatrix	));

	}



}

//========================================================================================
void AsStorage::Build_Temple1()
{
	//Ex = new AModel("models/GAME/obj/scene.gltf", glm::vec3(0.f, 2.5f, -5.f), glm::vec3(1.f),
		//glm::quat(1, 0, 0, 0));



}

//========================================================================================
void AsStorage::Buildings()
{

	//roman insula 1
	{
		GLfloat z_offset{}, x_offset{};
		std::vector <glm::mat4> instanceMatrix;
		int count = 0;


		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(16.5f, 4.5f, -18.4f);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);

		}

		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(-18.3f, 4.5f, 16.5f);
			glm::quat tempRotation = glm::quat(cos(glm::radians(90.0f / 2)), 0, sin(glm::radians(90.0f / 2)) * 1, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
		}

		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(-16.5f, 4.5f, -18.4f);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);

		}

		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(18.4f, 4.5f, 16.5f);
			glm::quat tempRotation = glm::quat(cos(glm::radians(90.0f / 2)) * 1, 0, sin(glm::radians(90.0f / 2)) * 1, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);

		}


		Models.push_back(new AModel("models/GAME/roman_insula/scene.gltf", glm::vec3(5.f, 3.6f, 3.5f), glm::vec3(3.f),
			glm::quat(0, 0, 0, 0), count, instanceMatrix));

	}

	//roman insula 2
	{
		std::vector <glm::mat4> instanceMatrix;
		int count = 0;

		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(9.5f, 4.5f, -18.5f);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
		}

		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(12.5f, 4.5f, 18.5f);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
		}

		Models.push_back(new AModel("models/GAME/roman_insula_2/scene.gltf", glm::vec3(5.f, 0.6f, 5.f), glm::vec3(3.f),
			glm::quat(0, 0, 0, 0), count, instanceMatrix));
	}

	//roman insula 3
	{
		std::vector <glm::mat4> instanceMatrix;
		int count = 0;

		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(15.7f, 4.5f, 16.4f);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
		}

		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(2.5f, 4.5f, -18.f);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
		}

		Models.push_back(new AModel("models/GAME/roman_insula_3/scene.gltf", glm::vec3(5.f, 0.6f, 5.f), glm::vec3(3.f),
			glm::quat(0, 0, 0, 0), count, instanceMatrix));
	}

	//roman insula 4
	{
		std::vector <glm::mat4> instanceMatrix;
		int count = 0;

		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(-19.5f, 4.5f, 10.7f);
			glm::quat tempRotation = glm::quat(cos(glm::radians(90.0f)) * 1, 0, sin(glm::radians(90.0f)) * 1, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
		}

		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(-11.f, 4.5f, -19.5f);
			glm::quat tempRotation = glm::quat(cos(glm::radians(90.0f) / 2) * 1, 0, sin(glm::radians(90.0f) / 2) * 1, 0);
			glm::vec3 tempScale = glm::vec3(4.f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
		}

		Models.push_back(new AModel("models/GAME/roman_insula_4/scene.gltf", glm::vec3(5.f, 0.6f, 5.f), glm::vec3(3.f),
			glm::quat(0, 0, 0, 0), count, instanceMatrix));
	}



}

//========================================================================================
void AsStorage::Build_Fances()
{
	{
		std::vector <glm::mat4> instanceMatrix;
		int count = 0;

		float offset = -15.5f;
		for (int i=0; i < 25; ++i)
			{
				++count;
				glm::vec3 tempTranslation = glm::vec3(offset, 0.5f, 19.5f);
				glm::quat tempRotation = glm::quat(cos(glm::radians(90.0f / 2)) * 1, 0, sin(glm::radians(90.0f / 2)) * 1, 0);
				glm::vec3 tempScale = glm::vec3(0.5f);

				glm::mat4 trans = glm::mat4(1.0f);
				glm::mat4 rot = glm::mat4(1.0f);
				glm::mat4 sca = glm::mat4(1.0f);

				// Transform the matrices to their correct form
				trans = glm::translate(trans, tempTranslation);
				rot = glm::mat4_cast(tempRotation);
				sca = glm::scale(sca, tempScale);

				instanceMatrix.push_back(trans * rot * sca);
				offset += 1.f;

				Scene->Collision_Level_Data[i + 4][0] = 1;
			}

		offset = 19.5f;
		for (int i = 0; i < 8; ++i)
		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(8.5, 0.5f, offset);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(0.5f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
			offset -= 1.f;

			Scene->Collision_Level_Data[28][i] = 1;
		}

		offset = 8.5f;
		for (int i = 0; i < 12; ++i)
		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(offset, 0.5f, 12.5);
			glm::quat tempRotation = glm::quat(cos(glm::radians(90.0f / 2)) * 1, 0, sin(glm::radians(90.0f / 2)) * 1, 0);
			glm::vec3 tempScale = glm::vec3(0.5f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
			offset += 1.f;

			Scene->Collision_Level_Data[28 + i][7] = 1;
		}

		offset = 12.5f;
		for (int i = 0; i < 29; ++i)
		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(19.5, 0.5f, offset);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(0.5f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
			offset -= 1.f;

			Scene->Collision_Level_Data[39][7 + i] = 1;
		}

		offset = -8.5f;
		for (int i = 0; i < 8; ++i)
		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(offset, 0.5f, -19.5f);
			glm::quat tempRotation = glm::quat(cos(glm::radians(90.0f / 2)) * 1, 0, sin(glm::radians(90.0f / 2)) * 1, 0);
			glm::vec3 tempScale = glm::vec3(0.5f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
			offset += 1.f;

			Scene->Collision_Level_Data[11 + i][39] = 1;
		}

		offset = 7.5f;
		for (int i = 0; i < 24; ++i)
		{
			++count;
			glm::vec3 tempTranslation = glm::vec3(-19.5f, 0.5f, offset);
			glm::quat tempRotation = glm::quat(0, 0, 0, 0);
			glm::vec3 tempScale = glm::vec3(0.5f);

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, tempTranslation);
			rot = glm::mat4_cast(tempRotation);
			sca = glm::scale(sca, tempScale);

			instanceMatrix.push_back(trans * rot * sca);
			offset -= 1.f;

			Scene->Collision_Level_Data[0][12 + i] = 1;
		}

		Models.push_back(new AModel("models/GAME/wall/scene.gltf", glm::vec3(5.f, 0.6f, 5.f), glm::vec3(3.f),
			glm::quat(0, 0, 0, 0), count, instanceMatrix));
	}
}

//========================================================================================
void AsStorage::Trees()
{
	std::vector <glm::mat4> instanceMatrix;
	int count = 0;
	float offset;

	offset = 0.f;
	for (int i = 0; i < 5; ++i)
	{
		++count;
		glm::vec3 tempTranslation = glm::vec3(-17.f + offset, 2.5f, -11.5);
		glm::quat tempRotation = glm::quat(0, 0, 0, 0);
		glm::vec3 tempScale = glm::vec3(0.5f);

		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);

		instanceMatrix.push_back(trans * rot * sca);
		offset += 5.f;

		Scene->Collision_Level_Data[2 + i * 5][31] = 1;

	}

	offset = -13.f;
	for (int i = 0; i < 5; ++i)
	{
		++count;
		glm::vec3 tempTranslation = glm::vec3(offset, 2.5f, 14.5);
		glm::quat tempRotation = glm::quat(0, 0, 0, 0);
		glm::vec3 tempScale = glm::vec3(0.5f);

		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);

		instanceMatrix.push_back(trans * rot * sca);
		offset += 5.f;

		Scene->Collision_Level_Data[6 + i * 5][5] = 1;

	}

	offset = 8.5f;
	for (int i = 0; i < 5; ++i)
	{
		++count;
		glm::vec3 tempTranslation = glm::vec3(13.f, 2.5f, offset);
		glm::quat tempRotation = glm::quat(0, 0, 0, 0);
		glm::vec3 tempScale = glm::vec3(0.5f);

		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);

		instanceMatrix.push_back(trans * rot * sca);
		offset -= 5.f;

		Scene->Collision_Level_Data[32][11 + i * 5] = 1;

	}

	Models.push_back(new AModel("models/GAME/tree_1/scene.gltf", glm::vec3(5.f, 0.6f, 5.f), glm::vec3(3.f),
		glm::quat(0, 0, 0, 0), count, instanceMatrix));
}

//========================================================================================

//========================================================================================
// private section:
//========================================================================================