#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include <glm/gtx/matrix_decompose.hpp>

//textures
#include<stb/stb_image.h>

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//SDL2 for musik
#include <SDL.h>
#undef main
#include <SDL_mixer.h>

#include<string>
#include<vector>
#include<map>

#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

class AsSettings
{
public:
	static const unsigned int Width = 800;
	static const unsigned int Height = 800;
	static const unsigned int Samples = 8;

	static const float Gamma;

	static double Delta_Time;

	static int Get_Random(int start, int range);
	static float Distance_Beetween_Two_Points(glm::vec3 pos_a, glm::vec3 pos_b);

private:
};


