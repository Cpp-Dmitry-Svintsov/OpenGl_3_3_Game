#include "plane.h"

//==============================================================================================================================
//static variaable
//==============================================================================================================================



//==============================================================================================================================
//destructor & constructor
//==============================================================================================================================

APlane::~APlane()
{
	Vao.Unbind();
	Vao.Delete();
}

APlane::APlane(glm::vec3 pos, float width, float height)
	:Pos(pos)
{
	float half_of_widt = width / 2.f;
	float half_of_height = height / 2.f;

	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES											/        COLORS         /    TexCoord       /       NORMALS      //
		pos.x - half_of_widt, pos.y,  pos.z + half_of_height,		0.0f, 0.0f, 0.0f,		0.0f,   0.0f,		0.0f, 1.0f, 0.0f,
		pos.x - half_of_widt, pos.y, pos.z - half_of_height,		0.0f, 0.0f, 0.0f,		0.0f,  10.0f,		0.0f, 1.0f, 0.0f,
		pos.x + half_of_widt, pos.y, pos.z - half_of_height,		0.0f, 0.0f, 0.0f,		10.0f, 10.0f,		0.0f, 1.0f, 0.0f,
		pos.x + half_of_widt, pos.y,  pos.z + half_of_height,		0.0f, 0.0f, 0.0f,		10.0f,  0.0f,		0.0f, 1.0f, 0.0f
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	// Generates Vertex Array Object and binds it
	Vao.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	AVBO vbo(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	AEBO ebo(indices, sizeof(indices));
	// Links VBO attributes such as coordinates and colors to VAO
	Vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	Vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	Vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	Vao.LinkAttrib(vbo, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	Vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
	vbo.Delete();
	ebo.Delete();
}

//==============================================================================================================================
//public section:
//==============================================================================================================================
void APlane::Draw(AShader& shaderProgram, ACamera& camera)
{
	//put postion of current plane into the shader
	Model = glm::translate(glm::mat4(1.f), Pos);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(Model));

	//bind buffer
	Vao.Bind();

	std::stringstream str;
	str.str(" ");

	if (Lights.size() > 0)
	{
		glm::vec4 lightColor(1.f);
		glm::vec3 lightPos(0.f);
		int i = 0;

		for (i; i < Lights.size(); ++i)
		{
			lightColor = Lights[i]->Color;
			lightPos = Lights[i]->Pos;

			str.str("");
			str << "colors[" << i << "];";

			glUniform4f(glGetUniformLocation(shaderProgram.ID, str.str().c_str() ), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			str.str("");
			str << "positions[" << i << "];";

			glUniform3f(glGetUniformLocation(shaderProgram.ID, str.str().c_str() ), lightPos.x, lightPos.y, lightPos.z);
		}
		str.str("");
		str << "light_counter = " << i << ";";
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "light_counter"), i + 1);
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

//==============================================================================================================================
//private section:
//==============================================================================================================================
void APlane::Add_Light_Object(ALite_Cube* obj)
{
	Lights.push_back(obj);
}
//==============================================================================================================================
