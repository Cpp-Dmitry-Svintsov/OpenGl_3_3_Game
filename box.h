#ifndef BOX_CLASS_H
#define BOX_CLASS_H

#include "mesh.h"

class ABOX
{
public:
	~ABOX();
	ABOX(glm::vec3 pos, glm::vec3 size, glm::vec3 color, ATexture* texture, glm::vec3 tex_offset = glm::vec3(0.f) );

	void Draw(AShader& shader_program, ACamera& camera);

private:
	glm::vec3 Pos;
	glm::vec3 Color;
	AMesh *Mesh;


	glm::mat4 Model;

	AVAO Vao;

	std::vector <ATexture*> Textures;

};

#endif // !BOX_CLASS_H

