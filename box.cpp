#include "box.h"

//=====================================================================================================================
// destructor & constructor
//=====================================================================================================================
ABOX::~ABOX()
{
	delete Mesh;
	Mesh = nullptr;

}

ABOX::ABOX(glm::vec3 pos, glm::vec3 size, glm::vec3 color, ATexture* texture, glm::vec3 tex_offset)
	:Pos(pos), Model(	glm::mat4(1.f)	)
{
	Textures = std::vector<ATexture*>{ texture };

	GLfloat hhy = (GLfloat)size.y / 2.f;
	GLfloat hhx = (GLfloat)size.x / 2.f;
	GLfloat hhz = (GLfloat)size.z / 2.f;

	//texture offset
	glm::vec3 t;
	if ( tex_offset == glm::vec3(0.f) )
		t = size;
	else
		t = tex_offset;

	//       7--------6
	//      /|       /|
	//     4--------5 |
	//     | |      | |
	//     |+|++++++|+|
	//     | 3------|-2
	//     |/       |/
	//	   0--------1
	

	std::vector<SVertex> box_vertices =
	{
		// positions													//Normals					//Colors		//texture coord
		//front
		SVertex{glm::vec3(pos.x - hhx, pos.y - hhy, pos.z + hhz),	glm::vec3(0.f, 0.f, 1.f),		color,		glm::vec2(0.f, 0.f)},
		SVertex{glm::vec3(pos.x - hhx, pos.y + hhy, pos.z + hhz),	glm::vec3(0.f, 0.f, 1.f),		color,		glm::vec2(0.f, t.y)},
		SVertex{glm::vec3(pos.x + hhx, pos.y + hhy, pos.z + hhz),	glm::vec3(0.f, 0.f, 1.f),		color,		glm::vec2(t.x, t.y)},
		SVertex{glm::vec3(pos.x + hhx, pos.y - hhy, pos.z + hhz),	glm::vec3(0.f, 0.f, 1.f),		color,		glm::vec2(t.x, 0.f)},
		// right
		SVertex{glm::vec3(pos.x + hhx, pos.y - hhy, pos.z + hhz),	glm::vec3(1.f, 0.f, 0.f),		color,		glm::vec2(0.f, 0.f)},
		SVertex{glm::vec3(pos.x + hhx, pos.y + hhy, pos.z + hhz),	glm::vec3(1.f, 0.f, 0.f),		color,		glm::vec2(0.f, t.y)},
		SVertex{glm::vec3(pos.x + hhx, pos.y + hhy, pos.z - hhz),	glm::vec3(1.f, 0.f, 0.f),		color,		glm::vec2(t.x, t.y)},
		SVertex{glm::vec3(pos.x + hhx, pos.y - hhy, pos.z - hhz),	glm::vec3(1.f, 0.f, 0.f),		color,		glm::vec2(t.x, 0.f)},
		// back
		SVertex{glm::vec3(pos.x + hhx, pos.y - hhy, pos.z - hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(0.f, 0.f)},
		SVertex{glm::vec3(pos.x + hhx, pos.y + hhy, pos.z - hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(0.f, t.y)},
		SVertex{glm::vec3(pos.x - hhx, pos.y + hhy, pos.z - hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(t.x, t.y)},
		SVertex{glm::vec3(pos.x - hhx, pos.y - hhy, pos.z - hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(t.x, 0.f)},
		// left
		SVertex{glm::vec3(pos.x - hhx, pos.y - hhy, pos.z - hhz),	glm::vec3(1.f, 0.f, 0.f),		color,		glm::vec2(0.f, 0.f)},
		SVertex{glm::vec3(pos.x - hhx, pos.y + hhy, pos.z - hhz),	glm::vec3(1.f, 0.f, 0.f),		color,		glm::vec2(0.f, t.y)},
		SVertex{glm::vec3(pos.x - hhx, pos.y + hhy, pos.z + hhz),	glm::vec3(1.f, 0.f, 0.f),		color,		glm::vec2(t.x, t.y)},
		SVertex{glm::vec3(pos.x - hhx, pos.y - hhy, pos.z + hhz),	glm::vec3(1.f, 0.f, 0.f),		color,		glm::vec2(t.x, 0.f)},
		// top
		SVertex{glm::vec3(pos.x - hhx, pos.y + hhy, pos.z + hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(0.f, 0.f)},
		SVertex{glm::vec3(pos.x - hhx, pos.y + hhy, pos.z - hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(0.f, t.z)},
		SVertex{glm::vec3(pos.x + hhx, pos.y + hhy, pos.z - hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(t.x, t.z)},
		SVertex{glm::vec3(pos.x + hhx, pos.y + hhy, pos.z + hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(t.x, 0.f)},
		// bottom
		SVertex{glm::vec3(pos.x - hhx, pos.y - hhy, pos.z - hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(0.f, 0.f)},
		SVertex{glm::vec3(pos.x - hhx, pos.y - hhy, pos.z + hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(0.f, t.z)},
		SVertex{glm::vec3(pos.x + hhx, pos.y - hhy, pos.z + hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(t.x, t.z)},
		SVertex{glm::vec3(pos.x + hhx, pos.y - hhy, pos.z - hhz),	glm::vec3(0.f, 1.f, 0.f),		color,		glm::vec2(t.x, 0.f)},
	};


	std::vector<GLuint> box_indices =
	{
		//front
		0, 1, 2,
		2, 3, 0,
		//right
		4, 5, 6,
		6, 7, 4,
		//back
		8, 9, 10,
		10, 11, 8,
		//left
		12, 13, 14,
		14, 15, 12,
		//top
		16, 17, 18,
		18, 19, 16,
		//bottom
		20, 21, 22,
		22, 23, 20
	};

	Mesh = new AMesh(box_vertices, box_indices, Textures);

}

//=====================================================================================================================
// public section:
//=====================================================================================================================
void ABOX::Draw(AShader& shader_program, ACamera& camera)
{

	//Model = glm::translate(Model, glm::vec3(0, Pos.y, Pos.z));
	//Model = glm::rotate(Model, 0.0001f, glm::vec3(0.f, 0.f, 1.f));
	//Model = glm::translate(Model, glm::vec3(0, -Pos.y, -Pos.z));

	Mesh->Draw(shader_program, camera, Model);
}

//=====================================================================================================================
