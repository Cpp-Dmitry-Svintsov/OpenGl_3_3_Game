#include "mesh_bones.h"

//===========================================================================================================================
// destructor & constructor
//===========================================================================================================================
AMesh_Bones::~AMesh_Bones()
{
}

AMesh_Bones::AMesh_Bones(std::vector <SVertex_Bones>& vertices, std::vector <GLuint>& indices, std::vector <ATexture*>& textures,
	unsigned int instancing, std::vector<glm::mat4> instanceMatrix)
	:Instancing(instancing)
{
	Vertices = vertices;
	Indices = indices;
	Textures = textures;

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	AVBO instanceVBO(instanceMatrix);
	AVBO vbo(vertices);
	// Generates Element Buffer Object and links it to indices
	AEBO ebo(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(SVertex_Bones), (void*)0); //Pos
	VAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(SVertex_Bones), (void*)(3 * sizeof(float))); // normals
	VAO.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(SVertex_Bones), (void*)(6 * sizeof(float))); //	colors
	VAO.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(SVertex_Bones), (void*)(9 * sizeof(float))); //	tex offsets

	VAO.LinkAttrib(vbo, 4, 4, GL_FLOAT, sizeof(SVertex_Bones), (void*)(11 * sizeof(float))); // bones weights

	vbo.Bind();
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(SVertex_Bones), (void*)(15 * sizeof(float))		);


	if (Instancing != 1)
	{
		instanceVBO.Bind();
		// Can't link to a mat4 so you need to link four vec4s
		VAO.LinkAttrib(instanceVBO, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
		VAO.LinkAttrib(instanceVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
		VAO.LinkAttrib(instanceVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		VAO.LinkAttrib(instanceVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		// Makes it so the transform is only switched when drawing the next instance
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
	}

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	instanceVBO.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

//===========================================================================================================================
// public section:
//===========================================================================================================================
void AMesh_Bones::Draw
(
	AShader& shader,
	ACamera& camera,
	std::vector <glm::mat4> bones_matrix,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{
	shader.Activate();
	VAO.Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < Textures.size(); i++)
	{
		std::string num;
		std::string type = Textures[i]->type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		Textures[i]->Tex_Unit(shader, (type + num).c_str(), i);
		Textures[i]->Bind();
	}
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");


	// Check if instance drawing should be performed
	if (Instancing == 1)
	{
		// Initialize matrices
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);


		// Transform the matrices to their correct form
		trans = glm::translate(trans, translation);
		rot = glm::mat4_cast(rotation);
		sca = glm::scale(sca, scale);

		// Push the matrices to the vertex shader
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

		//set bones matrix into the shader
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "bones_matrix"), bones_matrix.size(), GL_FALSE, glm::value_ptr(*bones_matrix.data()));

		//-------------------------------------------------


		// Draw the actual mesh
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawElementsInstanced(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0, Instancing);
	}
}

//===========================================================================================================================
