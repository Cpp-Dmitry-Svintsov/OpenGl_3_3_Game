#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include"Mesh.h"

#include<json/json.h>
using json = nlohmann::json;


class AModel
{
public:
	~AModel();
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	AModel(const char* file, glm::vec3 pos, glm::vec3 scale, glm::quat rot,
		unsigned int instancing = 1, std::vector<glm::mat4> instanceMatrix = {});

	void Draw(AShader& shader, ACamera& camera );

	glm::vec3 Pos;

private:
	// Loads a single mesh by its index
	void Load_Mesh(unsigned int indMesh);

	// Traverses a node recursively, so it essentially traverses all connected nodes
	void Traverse_Node(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	unsigned int Instancing;

	std::vector<glm::mat4> Instance_Matrix;

	// Gets the binary data from a file
	std::vector<unsigned char> Get_Data();
	// Interprets the binary data into floats, indices, and textures
	std::vector<float> Get_Floats(json accessor);
	std::vector<GLuint> Get_Indices(json accessor);
	std::vector<ATexture*> Get_Textures();

	// Assembles all the floats into vertices
	std::vector<SVertex> Assemble_Vertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// Variables for easy access
	const char* File;
	std::vector<unsigned char> Data;
	json JSON;

	// All the meshes and transformations
	std::vector<AMesh*> Meshes;
	std::vector<glm::vec3> Translations_Meshes;
	std::vector<glm::quat> Rotations_Meshes;
	std::vector<glm::vec3> Scales_Meshes;
	std::vector<glm::mat4> Matrices_Meshes;

	// Prevents textures from being loaded twice
	std::vector<std::string> Loaded_Tex_Name;
	std::vector<ATexture*> Loaded_Tex;

	// Helps with the assembly from above by grouping floats
	std::vector<glm::vec2> Group_Floats_Vec2(std::vector<float> floatVec);
	std::vector<glm::vec3> Group_Floats_Vec3(std::vector<float> floatVec);
	std::vector<glm::vec4> Group_Floats_Vec4(std::vector<float> floatVec);

	glm::mat4 Model;
	glm::mat4 Rot_Model;
	//glm::vec3 Pos;
	glm::quat Rot;
	glm::vec3 Scale;
};
#endif
