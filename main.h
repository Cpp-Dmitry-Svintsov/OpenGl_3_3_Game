#ifndef ENGINE_CLASS_H
#define ENGINE_CLASS_H


#include "scene.h"

class AsEngine
{
public:
	~AsEngine();
	AsEngine();

	int Init();
	void Run();

	void Update();
	void Render();
	void Render1();

private:
	void Get_FPS();
	void Update_Camera_Position();

	void Destructor();

	AShader *Main_Shader_Program;
	AShader* Main_Instance_Shader_Program;
	AShader *Skybox_Shader_Program;
	AShader *Frame_Buffer_Program;

	AShader* Bone_Shader_Program;

	AShader *Light_Shader_Program;
	AShader* Shadow_Map_Program;
	AShader* Shadow_Instance_Map_Program;

	AShader* Bone_Shadow_Program;

	AShader *Test_Shader_Program_1;
	AShader* Test_Shader_Program_2;

	std::vector<AShader*> Shader_Programs;
	std::vector<AShader*>::iterator Shader_Iterator;

	GLFWwindow* Window;
	ACamera Camera;
	AsStorage Storage;
	AsScene Scene;

	// Variables to create periodic event for FPS displaying
	double Prev_Time = 0.0;
	double Crnt_Time = 0.0;
	double Time_Diff{};
	// Keeps track of the amount of frames in timeDiff
	unsigned int Frame_Counter = 0;

	unsigned int Current_Sky_Box_Ui;


	//TEST
	glm::vec3 lightPos;
	unsigned int shadowMapFBO;
	unsigned int framebufferTexture;
	unsigned int shadowMapWidth = 2048, shadowMapHeight = 2048;
	unsigned int shadowMap;
	unsigned int FBO;
	unsigned int RBO;
	unsigned int postProcessingFBO;
	unsigned int postProcessingTexture;
	unsigned int rectVAO, rectVBO;

	glm::mat4 lightProjection;
};

#endif // !ENGINE_CLASS_H
