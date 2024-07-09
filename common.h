#pragma once

#include "settings.h"

//======================================================================================================================
// Reads a text file and outputs a string with everything in the text file
//======================================================================================================================
std::string Get_File_Contents(const char* filename);

//======================================================================================================================
// structure for save Vertices Buffer Parametrs
//======================================================================================================================
struct SVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Color;
	glm::vec2 Tex_Offset;
};

//======================================================================================================================
// structure for save Vertices Buffer Parametrs for Mesh + Bones
//======================================================================================================================
struct SVertex_Bones
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Color;
	glm::vec2 Tex_Offset;
	glm::vec4 Bones_Weight;
	glm::ivec4 Bones_Offset;
};

//======================================================================================================================
// structore for bones in skeleton
//======================================================================================================================
struct SBones_Node
{
	std::string Bone_Name;
	glm::mat4 *Model;
	glm::vec3 Par_Pos;
	glm::vec3 Pos;
	glm::quat Rot;
	std::vector<int> Childrends_Index;
};

//======================================================================================================================
// enum for key pressing
//======================================================================================================================
enum class EKey_Pressing
{
	Key_None,
	Key_Up,
	Key_Right,
	Key_Down,
	Key_Left,
};

//===============================================================================================================================
// pos elem for node elem structure
//===============================================================================================================================
struct SCoord
{
	SCoord();
	SCoord(int x, int y);

	bool operator<(const SCoord& l) const;
	bool operator==(const SCoord& other) const;
	bool operator!=(const SCoord& other) const;
	void operator=(const SCoord& other);
	void operator+(const SCoord& other);
	void operator+=(const SCoord& other);
	friend SCoord operator-(SCoord const &c1, SCoord const &c2);

	int X;
	int Y;
};

//===============================================================================================================================