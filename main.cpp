#include "main.h"

//========================================================================================================================================
// destroctor & constructor & init
//========================================================================================================================================
AsEngine::~AsEngine()
{}

//========================================================================================================================================
AsEngine::AsEngine()
{

}

//========================================================================================================================================
int AsEngine::Init()
{
	//init SDL2 library
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	
	//-----------------------------------		INIT GLFW SECTION		----------------------------------------------------//
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Only use this if you don't have a framebuffer
	//glfwWindowHint(GLFW_SAMPLES, samples);

	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	Window = glfwCreateWindow(AsSettings::Width, AsSettings::Height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(Window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, AsSettings::Width, AsSettings::Height);


	//-----------------------------------		SHADERS INIT SECTION		----------------------------------------------------//
	Main_Shader_Program = new AShader("default.vert", "default.frag");
	Main_Instance_Shader_Program = new AShader("default_instance.vert", "default.frag");
	Skybox_Shader_Program = new AShader("skybox.vert", "skybox.frag");
	Frame_Buffer_Program = new AShader("framebuffer.vert", "framebuffer.frag");

	Bone_Shader_Program = new AShader("bone.vert", "default.frag");

	Light_Shader_Program = new AShader("light.vert", "light.frag");
	Shadow_Map_Program = new AShader("shadow_map.vert", "shadow_map.frag");
	Shadow_Instance_Map_Program = new AShader("shadow_instance_map.vert", "shadow_map.frag");
	Bone_Shadow_Program = new AShader("bone_shadow.vert", "shadow_map.frag");

	Test_Shader_Program_1 = new AShader("test1.vert", "test1.frag");
	Test_Shader_Program_2 = new AShader("test2.vert", "test2.frag");

	Shader_Programs = std::vector<AShader*>{
		Main_Shader_Program,
		Main_Instance_Shader_Program,
		Skybox_Shader_Program,
		Frame_Buffer_Program,

		Bone_Shader_Program,

		Light_Shader_Program,
		Shadow_Map_Program,
		Shadow_Instance_Map_Program,
		Bone_Shadow_Program,

		Test_Shader_Program_1,
		Test_Shader_Program_2,
	};

	//-.-.-.--.-.-.--.-.-.--.-.-.--.-.-.--.-.-.--.-.-.--.-.-.--.-.-.--.-.-.--.-.-.-
	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightPos = glm::vec3(-1.f, 3.f, -1.f);

	Main_Shader_Program->Activate();
	glUniform4f(glGetUniformLocation(Main_Shader_Program->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(Main_Shader_Program->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Main_Instance_Shader_Program->Activate();
	glUniform4f(glGetUniformLocation(Main_Instance_Shader_Program->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(Main_Instance_Shader_Program->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Bone_Shader_Program->Activate();
	glUniform4f(glGetUniformLocation(Bone_Shader_Program->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(Bone_Shader_Program->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



	Frame_Buffer_Program->Activate();
	glUniform1i(glGetUniformLocation(Frame_Buffer_Program->ID, "screenTexture"), 0);
	glUniform1f(glGetUniformLocation(Frame_Buffer_Program->ID, "gamma"), AsSettings::Gamma);

	Test_Shader_Program_1->Activate();
	glUniform4f(glGetUniformLocation(Test_Shader_Program_1->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(Test_Shader_Program_1->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Test_Shader_Program_2->Activate();
	glUniform4f(glGetUniformLocation(Test_Shader_Program_2->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(Test_Shader_Program_2->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//-. - . - . -

	//-----------------------------------			ANOTHER SETTINGS			----------------------------------------------------//
	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Enables Multisampling
	glEnable(GL_MULTISAMPLE);

	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);


	//---------------------------------			FRAME BUFFER SETTINGS			--------------------------------------------------//

	float rectangleVertices[] =
	{
		//  Coords   // texCoords
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	// Prepare framebuffer rectangle VBO and VAO

	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	// Create Frame Buffer Object

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AsSettings::Samples, GL_RGB16F, AsSettings::Width, AsSettings::Height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	// Create Render Buffer Object
	
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, AsSettings::Samples, GL_DEPTH24_STENCIL8, AsSettings::Width, AsSettings::Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// Create Frame Buffer Object

	glGenFramebuffers(1, &postProcessingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	// Create Framebuffer Texture

	glGenTextures(1, &postProcessingTexture);
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, AsSettings::Width, AsSettings::Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

	// Error checking framebuffer
	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;


	// Framebuffer for Shadow Map

	glGenFramebuffers(1, &shadowMapFBO);

	// Texture for Shadow Map FBO


	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Prevents darkness outside the frustrum
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// Needed since we don't touch the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Matrices needed for the light's perspective
	glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
	glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightProjection = orthgonalProjection * lightView;


	Shadow_Map_Program->Activate();
	glUniformMatrix4fv(glGetUniformLocation(Shadow_Map_Program->ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

	Shadow_Instance_Map_Program->Activate();
	glUniformMatrix4fv(glGetUniformLocation(Shadow_Instance_Map_Program->ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));


	Bone_Shadow_Program->Activate();
	glUniformMatrix4fv(glGetUniformLocation(Bone_Shadow_Program->ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));


	Main_Instance_Shader_Program->Activate();
	glUniformMatrix4fv(glGetUniformLocation(Main_Instance_Shader_Program->ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));



	//---------------------------------			INIT MEMBERS SECTION			--------------------------------------------------//
	Storage.Init(&Scene);
	Scene.Init(&Storage);
	Scene.Player->Init(Window, &Camera, &Scene);
}

//========================================================================================================================================
// public section:
//========================================================================================================================================
void AsEngine::Run()
{
	// Main while loop
	while (!glfwWindowShouldClose(Window))
	{
		Get_FPS();

		Update();

		Camera.Inputs(Window);
		Camera.Update_Matrix(45.0f, 0.1f, 100.0f);

		Render();

		// Take care of all GLFW events
		glfwPollEvents();
	}

	glDeleteFramebuffers(1, &FBO);
	glDeleteFramebuffers(1, &postProcessingFBO);
	Destructor();
}

//===========================================================================================================================
void AsEngine::Update()
{
	Scene.Update();
}

//===========================================================================================================================
void AsEngine::Render()
{
	// Depth testing needed for Shadow Map
	glEnable(GL_DEPTH_TEST);

	// Preparations for the Shadow Map
	glViewport(0, 0, shadowMapWidth, shadowMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw scene for shadow map


	for (int i = 0; i < Storage.Models.size(); ++i)
		Storage.Models[i]->Draw(*Shadow_Instance_Map_Program, Camera);

	for (int i = 0; i < Storage.Boxes.size(); ++i)
		Storage.Boxes[i]->Draw(*Shadow_Map_Program, Camera);

	//Storage.Ex->Draw(*Shadow_Map_Program, Camera);

	Scene.Player->Draw(*Bone_Shadow_Program, Camera);

	// Switch back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Switch back to the default viewport
	glViewport(0, 0, AsSettings::Width, AsSettings::Height);
	// Bind the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// Specify the color of the background
	glClearColor(pow(0.07f, AsSettings::Gamma), pow(0.13f, AsSettings::Gamma), pow(0.17f, AsSettings::Gamma), 1.0f);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable depth testing since it's disabled when drawing the framebuffer rectangle
	glEnable(GL_DEPTH_TEST);

	Main_Shader_Program->Activate();
	glUniformMatrix4fv(glGetUniformLocation(Main_Shader_Program->ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));


	// Bind the Shadow Map
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(Main_Shader_Program->ID, "shadowMap"), 2);
	glUniform1i(glGetUniformLocation(Main_Instance_Shader_Program->ID, "shadowMap"), 2);


	// Draw the normal model on the map
	for (int i = 0; i < Storage.Models.size(); ++i)
		Storage.Models[i]->Draw(*Main_Instance_Shader_Program, Camera);

	//Draw player
	Scene.Player->Draw(*Bone_Shader_Program, Camera);

	for (int j{}; j < Scene.ProjectTiles.size(); ++j)
		Scene.ProjectTiles[j]->Draw(*Light_Shader_Program, Camera);


	for (int i = 0; i < Storage.Boxes.size(); ++i)
		Storage.Boxes[i]->Draw(*Main_Shader_Program, Camera);

	//Draw zombies
	for (int z{}; z < Scene.Zombies.size(); ++z)
		Scene.Zombies[z]->Draw(*Bone_Shader_Program, Camera);

	// Make it so the multisampling FBO is read while the post-processing FBO is drawn
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
	// Conclude the multisampling and copy it to the post-processing FBO
	glBlitFramebuffer(0, 0, AsSettings::Width, AsSettings::Height, 0, 0, AsSettings::Width, AsSettings::Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw the framebuffer rectangle
	Frame_Buffer_Program->Activate();
	glBindVertexArray(rectVAO);
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Swap the back buffer with the front buffer
	glfwSwapBuffers(Window);

}

//===========================================================================================================================
void AsEngine::Render1()
{
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera.Inputs(Window);
	Camera.Update_Matrix(45.0f, 0.1f, 100.0f);

	// Draw a model
	Storage.Models[1]->Draw(*Test_Shader_Program_1, Camera);

	// Swap the back buffer with the front buffer
	glfwSwapBuffers(Window);
}

//========================================================================================================================================
// private section:
//========================================================================================================================================
void AsEngine::Get_FPS()
{
	// Updates counter and times
	Crnt_Time = glfwGetTime();
	Time_Diff = Crnt_Time - Prev_Time;
	Frame_Counter++;
	AsSettings::Delta_Time = Time_Diff / Frame_Counter;

	if (Time_Diff >= 1.0 / 30.0)
	{
		// Creates new title
		std::string FPS = std::to_string((1.0 / Time_Diff) * Frame_Counter);
		std::string s = std::to_string(AsSettings::Delta_Time);
		std::string newTitle = FPS + "FPS / " + s + "s";
		glfwSetWindowTitle(Window, newTitle.c_str());

		// Resets times and counter
		Prev_Time = Crnt_Time;
		Frame_Counter = 0;

		// Use this if you have disabled VSync
		//camera.Inputs(window);
	}
}

//========================================================================================================================================
void AsEngine::Update_Camera_Position()
{
	for (int i = 0; i < Shader_Programs.size(); ++i)
	{
		glUniform3f(glGetUniformLocation(Shader_Programs[i]->ID, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);
		Camera.Matrix( *Shader_Programs[i] , "camMatrix");
	}
}

//========================================================================================================================================
void AsEngine::Destructor()
{

	for (Shader_Iterator = Shader_Programs.begin(); Shader_Iterator != Shader_Programs.end(); ++Shader_Iterator)
	{
		delete (*Shader_Iterator);
		Shader_Iterator = Shader_Programs.erase(Shader_Iterator);
		if (Shader_Iterator == Shader_Programs.end())
			break;
	}

	glDeleteFramebuffers(1, &postProcessingFBO);
	// Delete window before ending the program
	glfwDestroyWindow(Window);
	// Terminate GLFW before ending the program
	glfwTerminate();


	//destroy SDL2
	Mix_Quit();
	SDL_Quit();
}

//========================================================================================================================================
//========================================================================================================================================
//main func start the program
//========================================================================================================================================
//========================================================================================================================================

//global variables
AsEngine Engine;

int main()
{
	srand(time(NULL));

	Engine.Init();

	Engine.Run();

	return 0;
}