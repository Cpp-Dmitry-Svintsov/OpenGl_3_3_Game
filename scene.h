#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include "storage.h"
#include "path_finder.h"

enum class ESound_Type : unsigned char
{
	Shoot,
	Hit,
};

class AsScene
{
public:
	~AsScene();
	AsScene();

	void Init(AsStorage *storage);

	void Update();

	bool Check_Collision(glm::vec3 pos);

	void Get_Path(glm::vec3 start, glm::vec3 goal, std::vector<SCoord>& path);
	glm::vec3 Ceil_Reversed_Transform(SCoord ceil_pos);

	void Play_Sound(ESound_Type type);

	//ProjectTiles
	std::vector<AProject_Tile*> ProjectTiles;

	//Zonbies
	std::vector<AZombie*> Zombies;

	AsPlayer* Player;

	unsigned char Collision_Level_Data[40][40]{};

private:
	void Prepare_Level_Collision();
	void Spauners_Act();
	void Project_Tiles_Act();
	void Zombies_Act();
	SCoord Ceil_Pos_Transform(glm::vec3 pos);

	float Spaunes_Timer = 0.f;

	const int Max_Enemys = 10;
	std::vector<glm::vec3> Spauners;

	int Path_Finder_Indicator_Pointer = 0;

	AsStorage* Storage;
	APath_Finder Path_Finder;

};

#endif // !SCENE_CLASS_H

