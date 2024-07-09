#ifndef STORAGE_CLASS_H
#define STORAGE_CLASS_H

#include "model.h"
#include "model_bones.h"
#include "skybox.h"
#include "lite_cube.h"
#include "box.h"
#include "plane.h"

#include "player.h"
#include "objects.h"
#include "zombie.h"

class AsScene;

class AsStorage
{
public:
	~AsStorage();
	AsStorage();

	void Init(AsScene *scene);

	//3d models
	std::vector<AModel*> Models;

	AModel_Bones *Zombie_Model;
	AModel_Bones* Player_Model;


	//sky Bosex
	std::vector<ASky_Box*> Sky_Boxes;

	//planes
	std::vector<APlane*> Planes;

	//Boxes
	std::vector<ABOX*> Boxes;

	//Textures
	std::vector<ATexture*> Textures_Dif;
	std::vector<ATexture*> Textures_Spec;
	std::vector<ATexture*> Textures;

	//Lights
	std::vector<ALite_Cube*> Lights;

	//sounds
	Mix_Music* Sound_Main_Theme = NULL;
	Mix_Chunk* Sound_Shoot = NULL;
	Mix_Chunk* Sound_Hit = NULL;
	Mix_Chunk* Sound_Zombie[4];

private:
	void Build_Temple();
	void Build_Temple1();
	void Buildings();
	void Build_Fances();
	void Trees();

	ATexture *Mramor_Tex;
	ATexture *Terra_Tex;
	ATexture *Light_Cube_Tex;

	AsScene* Scene;
};

#endif // !STORAGE_CLASS_H
