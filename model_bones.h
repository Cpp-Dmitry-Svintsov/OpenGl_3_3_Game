#ifndef MODEL_BONES_CLASS_H
#define MODEL_BONES_CLASS_H

#include "mesh_bones.h"

#include<json/json.h>
using json = nlohmann::json;


class AModel_Bones
{
public:
	~AModel_Bones();
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	AModel_Bones(const char* file, glm::vec3 pos, glm::vec3 scale, glm::quat rot,
		unsigned int instancing = 1, std::vector<glm::mat4> instanceMatrix = {});

	void Draw(
		AShader& shader,
		ACamera& camera,
		std::vector <glm::mat4> bones_matrix,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);

	glm::vec3 Pos;
	glm::mat4 Model;
	glm::quat Rot;
	glm::vec3 Scale;

	std::vector<SBones_Node> Bones;
	std::vector<glm::mat4> Bones_Matrices;

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
	std::vector<int> Get_Integers(json accessor);
	std::vector<GLuint> Get_Indices(json accessor);
	std::vector<ATexture*> Get_Textures();

	void Get_Bones_Matrices(unsigned int bones_ind);
	void Rotate_Bone(glm::vec3 pos, glm::quat rot, int bones_child_index);
	void Rotate_Bone1();

	// Assembles all the floats into vertices
	std::vector<SVertex_Bones> Assemble_Vertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs,
		std::vector<glm::ivec4> bones_offset,
		std::vector<glm::vec4> bones_weight
	);

	// Variables for easy access
	const char* File;
	std::vector<unsigned char> Data;
	json JSON;

	// All the meshes and transformations
	std::vector<AMesh_Bones*> Meshes;

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

	std::vector<glm::ivec4> Group_Integers_Vec4(std::vector<int> floatVec);

	std::vector<glm::mat4> Group_Floats_Mat4(std::vector<float> floatVec);

	
	glm::mat4 Rot_Model;
	//glm::vec3 Pos;
};

#endif // !MODEL_BONES_CLASS_H
