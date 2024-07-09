//#pragma once
//#include "storage.h"
//
//class AsEngine
//{
//public:
//	~AsEngine();
//	AsEngine();
//
//	int Init();
//	void Run();
//
//	void Update();
//	void Render();
//
//private:
//	void Get_FPS();
//	void Update_Camera_Position();
//
//	void Destructor();
//
//	AShader Main_Shader_Program;
//	AShader Skybox_Shader_Program;
//	AShader Test_Shader_Program;
//	AShader Light_Shader_Program;
//	AShader Test_Shader_Program_1;
//	AShader Frame_Buffer_Program;
//	AShader Shadow_Map_Program;
//	AShader Shadow_Cube_Map_Program;
//
//	std::vector<AShader*> Shader_Programs;
//	std::vector<AShader*>::iterator Shader_Iterator;
//
//	GLFWwindow* Window;
//	ACamera Camera;
//	AsStorage Storage;
//
//	// Variables to create periodic event for FPS displaying
//	double Prev_Time = 0.0;
//	double Crnt_Time = 0.0;
//	double Time_Diff{};
//	// Keeps track of the amount of frames in timeDiff
//	unsigned int Frame_Counter = 0;
//
//	unsigned int Current_Sky_Box_Ui;
//
//
//	//TEST
//	unsigned int shadowMapWidth = 2048;
//	unsigned int shadowMapHeight = 2048;
//	unsigned int shadowMapFBO;
//	unsigned int FBO;
//	unsigned int pointShadowMapFBO;
//	unsigned int depthCubemap;
//	unsigned int postProcessingFBO;
//	unsigned int postProcessingTexture;
//	unsigned int rectVAO, rectVBO;
//};