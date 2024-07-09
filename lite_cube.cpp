#include "lite_cube.h"


//============================================================================================================================
// constructor & destructor
//============================================================================================================================
ALite_Cube::~ALite_Cube()
{}

ALite_Cube::ALite_Cube(glm::vec3 pos, glm::vec3 color, glm::vec3 scale, glm::mat4 model)
	:Pos(pos), Count_Of_Indices(0)
{
	Color = glm::vec4(color, 1.f);

	Model = model;
	

	GLfloat lightVertices[] =
	{ //     COORDINATES     //
		-0.1f * scale.x, -0.1f * scale.y,  0.1f * scale.z,
		-0.1f * scale.x, -0.1f * scale.y, -0.1f * scale.z,
		 0.1f * scale.x, -0.1f * scale.y, -0.1f * scale.z,
		 0.1f * scale.x, -0.1f * scale.y,  0.1f * scale.z,
		-0.1f * scale.x,  0.1f * scale.y,  0.1f * scale.z,
		-0.1f * scale.x,  0.1f * scale.y, -0.1f * scale.z,
		 0.1f * scale.x,  0.1f * scale.y, -0.1f * scale.z,
		 0.1f * scale.x,  0.1f * scale.y,  0.1f * scale.z
	};

	GLuint lightIndices[] =
	{
		2, 1, 0,
		3, 2, 0,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};

	Count_Of_Indices = sizeof(lightIndices) / sizeof(GLuint);

	Vao.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	AVBO vbo(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	AEBO ebo(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	Vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	Vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	vbo.Delete();
	ebo.Delete();
}

//============================================================================================================================
// public section:
//============================================================================================================================
void ALite_Cube::Draw(AShader& shader_program, ACamera& camera)
{
	//Model = glm::translate(glm::mat4(1.f), Pos);

	shader_program.Activate();
	Vao.Bind();

	// Take care of the camera Matrix

	camera.Matrix(shader_program, "camMatrix");

	shader_program.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader_program.ID, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glUniform4f(glGetUniformLocation(shader_program.ID, "lightColor"), Color.x, Color.y, Color.z, Color.w );

	glDrawElements(GL_TRIANGLES, Count_Of_Indices, GL_UNSIGNED_INT, 0);
}
//============================================================================================================================
