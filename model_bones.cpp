#include"model_bones.h"

//=====================================================================================================================================
// destructor & constructors
//=====================================================================================================================================
AModel_Bones::~AModel_Bones()
{
	for (auto* mesh : Meshes)
		delete mesh;

	for (auto* loaded_tex : Loaded_Tex)
		delete loaded_tex;

}

//=====================================================================================================================================
AModel_Bones::AModel_Bones(const char* file, glm::vec3 pos, glm::vec3 scale, glm::quat rot,
	unsigned int instancing, std::vector<glm::mat4> instanceMatrix)
	:Pos(pos), Scale(scale), Rot(rot), Instancing(instancing), Instance_Matrix(instanceMatrix)
{
	// Make a JSON object
	std::string text = Get_File_Contents(file);
	JSON = json::parse(text);

	// Get the binary data
	File = file;
	Data = Get_Data();


	// Traverse all nodes
	unsigned int node_value = JSON["scenes"][0]["nodes"][0];

	Traverse_Node(node_value - 1);


	//Model transsformation:
	Model = glm::mat4(1.f);

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot1 = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	trans = glm::translate(trans, Pos);
	rot1 = glm::mat4_cast(Rot);
	sca = glm::scale(sca, Scale);

	// Multiply all matrices together
	Model = Model * trans * rot1 * sca;

	//Get_Matrices
	//matrix of a bones
	unsigned int bonesMatrixAccInd = JSON["skins"][0]["inverseBindMatrices"];
	Get_Bones_Matrices(bonesMatrixAccInd);

}

//===========================================================================================================================================
void AModel_Bones::Draw(
	AShader& shader,
	ACamera& camera,
	std::vector <glm::mat4> bones_matrix,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{

	for (unsigned int i = 0; i < Meshes.size(); i++)
	{
		//Meshes[i]->Draw(shader, camera, Matrices_Meshes[i]);
		Meshes[i]->Draw(shader, camera, bones_matrix, matrix);
		//Meshes[i]->Draw(shader, camera, glm::mat4(1.f), Pos, Rot, Scale);
	}
}

//===========================================================================================================================================
void AModel_Bones::Load_Mesh(unsigned int indMesh)
{
	// Get all accessor indices
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];

	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];


	//bones offset and bones weight for all vertices
	unsigned int bonesOffsetAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["JOINTS_0"];
	unsigned int bonesWeightAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["WEIGHTS_0"];

	// Use accessor indices to get all vertices components
	std::vector<float> posVec = Get_Floats(JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = Group_Floats_Vec3(posVec);
	std::vector<float> normalVec = Get_Floats(JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = Group_Floats_Vec3(normalVec);
	std::vector<float> texVec = Get_Floats(JSON["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = Group_Floats_Vec2(texVec);

	std::vector<int> bonesVec = Get_Integers(JSON["accessors"][bonesOffsetAccInd]);
	std::vector<glm::ivec4> bonesOffsetUVs = Group_Integers_Vec4(bonesVec);
	std::vector<float> weightVec = Get_Floats(JSON["accessors"][bonesWeightAccInd]);
	std::vector<glm::vec4> bonesWeightUVs = Group_Floats_Vec4(weightVec);

	// Combine all the vertex components and also get the indices and textures
	std::vector<SVertex_Bones> vertices = Assemble_Vertices(positions, normals, texUVs, bonesOffsetUVs, bonesWeightUVs);

	std::vector<GLuint> indices = Get_Indices(JSON["accessors"][indAccInd]);
	std::vector<ATexture*> textures = Get_Textures();

	// Combine the vertices, indices, and textures into a mesh
	Meshes.push_back(new AMesh_Bones(vertices, indices, textures,
		Instancing, Instance_Matrix));
}

void AModel_Bones::Traverse_Node(unsigned int nextNode, glm::mat4 matrix)
{
	// Current node
	json node = JSON["nodes"][nextNode];

	// Get translation if it exists
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = glm::make_vec3(transValues);
	}
	// Get quaternion if it exists
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}
	// Get scale if it exists
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}
	// Get matrix if it exists
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, Scale);

	// Multiply all matrices together
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	// Check if the node contains a mesh and if it does load it
	if (node.find("mesh") != node.end())
	{
		Translations_Meshes.push_back(translation);
		Rotations_Meshes.push_back(rotation);
		Scales_Meshes.push_back(scale);
		Matrices_Meshes.push_back(matNextNode);

		Load_Mesh(node["mesh"]);
	}

	// Check if the node has children, and if it does, apply this function to them with the matNextNode
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			Traverse_Node(node["children"][i], matNextNode);
	}
}

std::vector<unsigned char> AModel_Bones::Get_Data()
{
	// Create a place to store the raw text, and get the uri of the .bin file
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	// Store raw text data into bytesText
	std::string fileStr = std::string(File);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = Get_File_Contents((fileDirectory + uri).c_str());

	// Transform the raw text data into bytes and put them in a vector
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> AModel_Bones::Get_Floats(json accessor)
{
	std::vector<float> floatVec;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];
	//unsigned int byteOffset = bufferView.value("byteOffset", 0);

	// Interpret the type and store it into numPerVert
	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else if (type == "MAT4") numPerVert = 16;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	// Go over all the bytes in the data at the correct place using the properties from above
	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		unsigned char bytes[] = { Data[i++], Data[i++], Data[i++], Data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

//=====================================================================================================================
std::vector<int> AModel_Bones::Get_Integers(json accessor)
{
	std::vector<int> intVec;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];
	//unsigned int byteOffset = bufferView.value("byteOffset", 0);

	// Interpret the type and store it into numPerVert
	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	// Go over all the bytes in the data at the correct place using the properties from above
	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * numPerVert;

	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; ++i)
	{
		intVec.push_back(Data[i]);
	}

	return intVec;
}


//=====================================================================================================================
std::vector<GLuint> AModel_Bones::Get_Indices(json accessor)
{
	std::vector<GLuint> indices;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	//unsigned int byteOffset = bufferView["byteOffset"];
	unsigned int byteOffset = bufferView.value("byteOffset", 0);

	// Get indices with regards to their type: unsigned int, unsigned short, or short
	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { Data[i++], Data[i++], Data[i++], Data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { Data[i++], Data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { Data[i++], Data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

//=================================================================================================================================
std::vector<ATexture*> AModel_Bones::Get_Textures()
{
	std::vector<ATexture*> textures;

	std::string fileStr = std::string(File);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Go over all images
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		// uri of current texture
		std::string texPath = JSON["images"][i]["uri"];

		// Check if the texture has already been loaded
		bool skip = false;
		for (unsigned int j = 0; j < Loaded_Tex_Name.size(); j++)
		{
			if (Loaded_Tex_Name[j] == texPath)
			{
				textures.push_back(Loaded_Tex[j]);
				skip = true;
				break;
			}
		}

		// If the texture has been loaded, skip this
		if (!skip)
		{
			// Load diffuse texture
			if (texPath.find("baseColor") != std::string::npos || texPath.find("diffuse") != std::string::npos)
			{
				ATexture* diffuse = new ATexture((fileDirectory + texPath).c_str(), "diffuse", Loaded_Tex.size());
				textures.push_back(diffuse);
				Loaded_Tex.push_back(diffuse);
				Loaded_Tex_Name.push_back(texPath);
			}
			// Load specular texture
			else if (texPath.find("metallicRoughness") != std::string::npos || texPath.find("specular") != std::string::npos)
			{
				ATexture* specular = new ATexture((fileDirectory + texPath).c_str(), "specular", Loaded_Tex.size());
				textures.push_back(specular);
				Loaded_Tex.push_back(specular);
				Loaded_Tex_Name.push_back(texPath);
			}
		}
	}

	return textures;
}

//============================================================================================================================
void AModel_Bones::Get_Bones_Matrices(unsigned int bones_ind)
{
	std::stringstream my_str;
	my_str.str(" ");

	std::vector<glm::mat4> matrices;
	
	
	std::vector<float> bonesMatrixVec = Get_Floats(JSON["accessors"][bones_ind]);
	matrices = Group_Floats_Mat4(bonesMatrixVec);

	my_str << "============================================" << "BONES " << "============================================" << "\n";




	for (int i = 0; i < matrices.size(); ++i)
	{
		my_str << "============================================" << "BONE - " << i << "==========================================" << "\n";

		//bonesMatrixUVs1[i] = glm::transpose(glm::inverse(bonesMatrixUVs1[i])) * glm::mat4(1.f);
		//bonesMatrixUVs1[i] = glm::inverse(bonesMatrixUVs1[i]) * glm::mat4(1.f);

		//inverse matrices
		glm::mat4 new_inverse_mat(1.f);
		for (int row = 0; row < 4; row++)
		{
			for (int col{}; col < 4; col++)
			{
				new_inverse_mat[3 - row][3 - col] = matrices[i][row][col];
			}
		}

		//and save it back 
		matrices[i] = new_inverse_mat;

		//printed matrix
	
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(matrices[i], scale, rotation, translation, skew, perspective);
		
		rotation = glm::conjugate(rotation);


		std::string name;
		try
		{
			name = JSON["nodes"][matrices.size() - 1 - i]["name"];
		}
		catch (int e)
		{
			name = "unnamed";
		}


		glm::mat4 new_mat = glm::mat4(1.f);

		Bones_Matrices.push_back(new_mat);

		SBones_Node new_bone;
	
		new_bone.Par_Pos = translation;
		new_bone.Bone_Name = name;

		Bones.push_back(new_bone);

		my_str << "Name:	" << new_bone.Bone_Name << "\n";
		my_str << "Pos: " << Bones[i].Par_Pos.x << ",\t" << Bones[i].Par_Pos.y << ",\t" << Bones[i].Par_Pos.z << "\n";
		my_str << "Bone id " << i << "  has chaid - " << 0 << "\n";
		my_str << "\n";
	}


	//printf(my_str.str().c_str());


}

//============================================================================================================================
void AModel_Bones::Rotate_Bone(glm::vec3 pos, glm::quat rot, int bone_index)
{
	Bones_Matrices[bone_index] = glm::translate(Bones_Matrices[bone_index], -pos);
	Bones_Matrices[bone_index] = glm::rotate(Bones_Matrices[bone_index], rot.w, glm::vec3(rot.x, rot.y, rot.z));
	Bones_Matrices[bone_index] = glm::translate(Bones_Matrices[bone_index], pos);
	//if (Bones[bone_index].Childrends_Index[0] > 0)
	//	for (int child = 0; child < Bones[bone_index].Childrends_Index.size(); ++child)
	//	{
	//		Rotate_Bone(pos, rot, Bones[bone_index].Childrends_Index[child]);
	//	}
}

//============================================================================================================================
void AModel_Bones::Rotate_Bone1()
{

	int bone_id = 1;
	glm::quat rot(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));


	Bones_Matrices[bone_id] = glm::translate(Bones_Matrices[bone_id], -Bones[bone_id].Par_Pos);
	Bones_Matrices[bone_id] = glm::rotate(Bones_Matrices[bone_id],
		rot.w, glm::vec3(rot.x, rot.y, rot.z));
	Bones_Matrices[bone_id] = glm::translate(Bones_Matrices[bone_id], Bones[bone_id].Par_Pos);

	Rotate_Bone(Bones[bone_id].Par_Pos, rot, Bones[bone_id].Childrends_Index[0]);

	bone_id = 2;
	rot = glm::quat(glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));

	Bones_Matrices[bone_id] = glm::translate(Bones_Matrices[bone_id], -Bones[bone_id].Par_Pos);
	Bones_Matrices[bone_id] = glm::rotate(Bones_Matrices[bone_id],
		rot.w, glm::vec3(rot.x, rot.y, rot.z));
	Bones_Matrices[bone_id] = glm::translate(Bones_Matrices[bone_id], Bones[bone_id].Par_Pos);

	Rotate_Bone(Bones[bone_id].Par_Pos, rot, Bones[bone_id].Childrends_Index[0]);

	bone_id = 3;
	rot = glm::quat(glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));

	Bones_Matrices[bone_id] = glm::translate(Bones_Matrices[bone_id], -Bones[bone_id].Par_Pos);
	Bones_Matrices[bone_id] = glm::rotate(Bones_Matrices[bone_id],
		rot.w, glm::vec3(rot.x, rot.y, rot.z));
	Bones_Matrices[bone_id] = glm::translate(Bones_Matrices[bone_id], Bones[bone_id].Par_Pos);

}

//============================================================================================================================
std::vector<SVertex_Bones> AModel_Bones::Assemble_Vertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs,
	std::vector<glm::ivec4> bones_offset,
	std::vector<glm::vec4> bones_weight
)
{
	std::vector<SVertex_Bones> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			SVertex_Bones
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i],
				bones_weight[i],
				bones_offset[i]
			}
		);
	}
	return vertices;
}

//============================================================================================================================
std::vector<glm::vec2> AModel_Bones::Group_Floats_Vec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

//============================================================================================================================
std::vector<glm::vec3> AModel_Bones::Group_Floats_Vec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

//============================================================================================================================
std::vector<glm::vec4> AModel_Bones::Group_Floats_Vec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

//============================================================================================================================
std::vector<glm::ivec4> AModel_Bones::Group_Integers_Vec4(std::vector<int> floatVec)
{
	std::vector<glm::ivec4> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::ivec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

//============================================================================================================================
std::vector<glm::mat4> AModel_Bones::Group_Floats_Mat4(std::vector<float> floatVec)
{
	std::vector<glm::mat4> vector;

	for (int i = 0; i < floatVec.size(); i)
	{
		vector.push_back(glm::mat4(
			floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++],
			floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++],
			floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++],
			floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]
		));
	}

	return vector;
}

//============================================================================================================================
