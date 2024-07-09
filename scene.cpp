#include "scene.h"


//=======================================================================================================
// destructor & constructor
//=======================================================================================================
AsScene::~AsScene()
{
	for (auto* el : ProjectTiles)
		delete el;
	ProjectTiles.erase(ProjectTiles.begin(), ProjectTiles.end());

	for (auto* el : Zombies)
		delete el;
	Zombies.erase(Zombies.begin(), Zombies.end());

	delete Player;

}

//=======================================================================================================
AsScene::AsScene()
{}

//=======================================================================================================
//init
//=======================================================================================================
void AsScene::Init(AsStorage* storage)
{
	Storage = storage;

	Player = new AsPlayer(Storage->Player_Model, glm::vec3(0.5f, 0.5f, 0.5f), glm::quat(0.f, 0, 0, 0.f), glm::vec3(0.25f));


	//Zombies.push_back(	new AZombie(	Storage->Zombie_Model, glm::vec3(	-5.5f, 0.5f, 0.5f), glm::quat(0.f, 0, 0, 0.f), glm::vec3(0.27f)	)		);
	//Zombies[0]->Init(Player, this);

	std::stringstream ss;
	//ss << AsSettings::Distance_Beetween_Two_Points(Zombies[0]->Get_Pos(), Zombies[1]->Get_Pos());
	printf(ss.str().c_str());


	//
	Prepare_Level_Collision();

	//add spaunders
	Spauners.push_back(glm::vec3(18.5f, 0.5f, 11.5f));
	Spauners.push_back(glm::vec3(7.5f, 0.5f, 18.5f));
	Spauners.push_back(glm::vec3(3.5f, 0.5f, 18.5f));
	Spauners.push_back(glm::vec3(-1.5f, 0.5f, 18.5f));
	Spauners.push_back(glm::vec3(-5.5f, 0.5f, 18.5f));
	Spauners.push_back(glm::vec3(-11.5f, 0.5f, 18.5f));
	Spauners.push_back(glm::vec3(-15.5f, 0.5f, 18.5f));
	for (int i = 0; i < 6; ++i)
		Spauners.push_back(glm::vec3(-18.5f, 0.5f, 7.5f - ((float)i * 4.f)));
	for (int i = 0; i < 2; ++i)
		Spauners.push_back(glm::vec3(	-7.5f + i * 4.f, 0.5f, -18.5f	)		);
	for (int i = 0; i < 7; ++i)
		Spauners.push_back(glm::vec3(18.5f, 0.5f, -13.5f + i * 4.f));
	Spauners.push_back(glm::vec3(13.5f, 0.5f, 11.5f));
	for (int i = 0; i < 2; ++i)
		Spauners.push_back(glm::vec3(7.5f, 0.5f, 11.5f + i * 4.f	));


	//Prepare Path_Finder
	Path_Finder.Get_Graf(*Collision_Level_Data);

}

//=======================================================================================================
//public section:
//=======================================================================================================
void AsScene::Update()
{
	Spauners_Act();

	Player->Update();

	//update projectTiles
	Project_Tiles_Act();


	//update zombies
	Zombies_Act();


	//path_finder act
	int size = Zombies.size();
	if (Zombies.size() > 0)
	{
		if (Path_Finder_Indicator_Pointer > Zombies.size() - 1)
			Path_Finder_Indicator_Pointer = 0;

		Zombies[Path_Finder_Indicator_Pointer]->Update_Path();

		++Path_Finder_Indicator_Pointer;
	}
}

bool AsScene::Check_Collision(glm::vec3 pos)
{
	SCoord ceil_pos = Ceil_Pos_Transform(pos);

	if (Collision_Level_Data[ceil_pos.Y][ceil_pos.X] == 0)
		return false;
	else
		return true;
}

//=======================================================================================================
void AsScene::Get_Path(glm::vec3 start, glm::vec3 goal, std::vector<SCoord>& path)
{
	SCoord st_pos = Ceil_Pos_Transform(start);
	SCoord gol_pos = Ceil_Pos_Transform(goal);

	Path_Finder.Get_Path(st_pos, gol_pos, path);
}

//=======================================================================================================
glm::vec3 AsScene::Ceil_Reversed_Transform(SCoord pos)
{
	float z{};
	float x{};
	float y{};

	if (pos.X <= 19)
		z = 20.f - (float)pos.X;
	else
		z = 20.f - (float)pos.X;


	if (pos.Y <= 19)
		x = (float)pos.Y - 20.f;
	else
		x = (float)pos.Y - 20.f;

	return glm::vec3(x, y, z);
}

//=======================================================================================================
void AsScene::Play_Sound(ESound_Type type)
{
	switch (type)
	{
	case ESound_Type::Shoot:
		Mix_PlayChannel(-1, Storage->Sound_Shoot, 0);
		break;
	case ESound_Type::Hit:
		Mix_PlayChannel(-1, Storage->Sound_Hit, 0);
		break;
	default:
		break;
	}
}

//=======================================================================================================
// private section:
//=======================================================================================================
void AsScene::Prepare_Level_Collision()
{
	int i{}, j{};
	//tample 
	j = 12;
	for (i = 0; i < 4; ++i)
	{
		Collision_Level_Data[4][j] = 1;
		Collision_Level_Data[24][j] = 1;
		j += 5;
	}
	j = 9;
	for (i = 0; i < 3; ++i)
	{
		Collision_Level_Data[j][12] = 1;
		Collision_Level_Data[j][27] = 1;
		j += 5;
	}


	//gorgulie
	j = 14;
	for (i = 0; i < 3; ++i)
	{
		Collision_Level_Data[4][j] = 1;
		Collision_Level_Data[4][j+1] = 1;
		j += 5;
	}

	//buildings
	for(i=0; i < 4; ++i)
		for(j=0; j<12;++j)
			Collision_Level_Data[i][j] = 1;

	for (i = 36; i < 40; ++i)
		for (j = 0; j < 12; ++j)
		{
			Collision_Level_Data[j][i] = 1;
		}

	for (i = 26; i < 40; ++i)
		for (j = 36; j < 40; ++j)
		{
			Collision_Level_Data[i][j] = 1;
		}

	for (i = 19; i < 27; ++i)
		for (j = 38; j < 40; ++j)
		{
			Collision_Level_Data[i][j] = 1;
		}
}

//=======================================================================================================
void AsScene::Spauners_Act()
{
	Spaunes_Timer += AsSettings::Delta_Time;

	if (Spaunes_Timer > 5.f)
	{
		int random_index = AsSettings::Get_Random(0, Spauners.size());
		if (Zombies.size() < Max_Enemys)
		{
			AZombie* new_zombie = new AZombie(Storage->Zombie_Model, Spauners[random_index], glm::quat(0.f, 0, 0, 0.f), glm::vec3(0.27f));
			new_zombie->Init(Player, this);
			Zombies.push_back(new_zombie);

			//play random sound
			int rand_ind = AsSettings::Get_Random(0, 4);
			Mix_PlayChannel(-1, Storage->Sound_Zombie[rand_ind], 0);


		}
		Spaunes_Timer = 0.f;
	}
}

//=======================================================================================================
void AsScene::Project_Tiles_Act()
{
	std::vector<AProject_Tile*>::iterator it;
	for (it = ProjectTiles.begin(); it != ProjectTiles.end(); ++it)
	{
		(*it)->Update();

		//check hit zombies
		for (int i{}; i < Zombies.size(); ++i)
		{
			if (AsSettings::Distance_Beetween_Two_Points(Zombies[i]->Get_Pos(), (*it)->Get_Pos()) < Zombies[i]->Radius)
			{
				(*it)->Is_Alive = false;
				Zombies[i]->Is_Alive = false;
				Play_Sound(ESound_Type::Hit);
			}
		}



		if (!(*it)->Is_Alive)
		{
			delete (*it);
			it = ProjectTiles.erase(it);
			if (it == ProjectTiles.end())
				break;
		}
	}
}

//=======================================================================================================
void AsScene::Zombies_Act()
{
	std::vector<AZombie*>::iterator it;
	for (it = Zombies.begin(); it != Zombies.end(); ++it)
	{
		(*it)->Update();


		if (!(*it)->Is_Alive)
		{
			delete (*it);
			it = Zombies.erase(it);
			if (it == Zombies.end())
				break;
		}
	}
}

//=======================================================================================================
SCoord AsScene::Ceil_Pos_Transform(glm::vec3 pos)
{
	int ceil_z{};
	int ceil_x{};

	if (pos.x < 0)
		ceil_x = (int)pos.x + 19;
	else
		ceil_x = (int)pos.x + 20;


	if (pos.z >= 0.f)
		ceil_z = 19 - (int)pos.z;
	else
		ceil_z = (20 + (int)pos.z * -1);

	return SCoord(ceil_z, ceil_x);
}

//=======================================================================================================
