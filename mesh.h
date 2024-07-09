#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"
#include "common.h"

class AMesh
{
public:
	~AMesh();
	AMesh(std::vector<SVertex>& vertices, std::vector<GLuint>& indices, std::vector <ATexture*>& textures,
		unsigned int instancing = 1, std::vector<glm::mat4> instanceMatrix = {});

	// Draws the mesh
	void Draw
	(
		AShader& shader,
		ACamera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);

	std::vector <SVertex> Vertices;
	std::vector <GLuint> Indices;
	std::vector <ATexture*> Textures;

private:
	unsigned int Instancing;

	AVAO VAO;
};
#endif
