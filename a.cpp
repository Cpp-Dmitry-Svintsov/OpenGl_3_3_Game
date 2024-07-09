//#include "a.h"
//
//
//// Number of samples per pixel for MSAA
//unsigned int samples = 8;
//
//// Controls the gamma function
//float gamma = 2.2f;
//
//
//float rectangleVertices[] =
//{
//	//  Coords   // texCoords
//	 1.0f, -1.0f,  1.0f, 0.0f,
//	-1.0f, -1.0f,  0.0f, 0.0f,
//	-1.0f,  1.0f,  0.0f, 1.0f,
//
//	 1.0f,  1.0f,  1.0f, 1.0f,
//	 1.0f, -1.0f,  1.0f, 0.0f,
//	-1.0f,  1.0f,  0.0f, 1.0f
//};
//
//const unsigned int width = 800;
//const unsigned int height = 800;
//
//
////===================================================================================================
//// destructor & constructor
////===================================================================================================
//AsEngine::~AsEngine()
//{
//}
//
//AsEngine::AsEngine()
//	:Current_Sky_Box_Ui(0)
//{
//
//}
//
////===================================================================================================
//// initialization:
////===================================================================================================
//int AsEngine::Init()
//{
//	// Initialize GLFW
//	glfwInit();
//
//	// Tell GLFW what version of OpenGL we are using 
//	// In this case we are using OpenGL 3.3
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	// Only use this if you don't have a framebuffer
//	//glfwWindowHint(GLFW_SAMPLES, 8);
//	// Tell GLFW we are using the CORE profile
//	// So that means we only have the modern functions
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
//	Window = glfwCreateWindow(AsSettings::Width, AsSettings::Height, "YoutubeOpenGL", NULL, NULL);
//	// Error check if the window fails to create
//	if (Window == NULL)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//	// Introduce the window into the current context
//	glfwMakeContextCurrent(Window);
//
//	//Load GLAD so it configures OpenGL
//	gladLoadGL();
//	// Specify the viewport of OpenGL in the Window
//	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
//	glViewport(0, 0, AsSettings::Width, AsSettings::Height);
//
//
//	//-----------------------------------------------------------------------------------------
//	// shader init section:
//	//-----------------------------------------------------------------------------------------
//
//	// Generates Shader object 
//	Main_Shader_Program.Init("default.vert", "default.frag");
//	Skybox_Shader_Program.Init("skybox.vert", "skybox.frag");
//	Test_Shader_Program.Init("test.vert", "test.frag");
//	Light_Shader_Program.Init("light.vert", "light.frag");
//	Test_Shader_Program_1.Init("test1.vert", "test1.frag");
//	Frame_Buffer_Program.Init("framebuffer.vert", "framebuffer.frag");
//	Shadow_Map_Program.Init("shadow_map.vert", "shadow_map.frag");
//	//Shadow_Cube_Map_Program.Init("shadow_cube_map.vert", "shadow_cube_map.frag", "shadow_cube_map.geom");
//
//	Shader_Programs = std::vector<AShader*>{
//		&Main_Shader_Program,
//		&Skybox_Shader_Program,
//		&Test_Shader_Program,
//		&Light_Shader_Program,
//		&Test_Shader_Program_1,
//		&Frame_Buffer_Program,
//		&Shadow_Map_Program,
//		&Shadow_Cube_Map_Program,
//	};
//
//	//-----------------------------------------------------------------------
//	// settings for face cutting:
//	//-----------------------------------------------------------------------
//
//	// Enables the Depth Buffer
//	glEnable(GL_DEPTH_TEST);
//
//	// Enables Multisampling
//	glEnable(GL_MULTISAMPLE);
//
//	// Enables Cull Facing
//	glEnable(GL_CULL_FACE);
//	// Keeps front faces
//	glCullFace(GL_FRONT);
//	// Uses counter clock-wise standard
//	glFrontFace(GL_CCW);
//
//	//-----------------------------------------------------------------------
//	// camera initialization:
//	//-----------------------------------------------------------------------
//	Camera.Init(glm::vec3(0.0f, 0.0f, 2.0f));
//
//	//-----------------------------------------------------------------------
//	// storage: !! This section must be delete and transfer into enother instance
//	//-----------------------------------------------------------------------
//
//	Storage.Init();
//	//set cur sky_box
//	//Skybox_Shader_Program.Activate();
//	//glUniform1i(glGetUniformLocation(Skybox_Shader_Program.Get_Program_Id(), "skybox"), 0);
//	Current_Sky_Box_Ui = 0;
//
//
//	//for test--------------------------------------------------------------------------------------
//	int width = AsSettings::Width;
//	int height = AsSettings::Height;
//
//	glm::vec4 lightColor = Storage.Lights[0]->Color;
//	glm::vec3 lightPos = Storage.Lights[0]->Pos;
//	Frame_Buffer_Program.Activate();
//	glUniform1i(glGetUniformLocation(Frame_Buffer_Program.Get_Program_Id(), "screenTexture"), 0);
//	glUniform1f(glGetUniformLocation(Frame_Buffer_Program.Get_Program_Id(), "gamma"), gamma);
//
//	// Prepare framebuffer rectangle VBO and VAO
//
//	glGenVertexArrays(1, &rectVAO);
//	glGenBuffers(1, &rectVBO);
//	glBindVertexArray(rectVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//
//	// Create Frame Buffer Object
//
//	glGenFramebuffers(1, &FBO);
//	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
//
//	// Create Framebuffer Texture
//	unsigned int framebufferTexture;
//	glGenTextures(1, &framebufferTexture);
//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
//	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F, width, height, GL_TRUE);
//	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
//	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);
//
//	// Create Render Buffer Object
//	unsigned int RBO;
//	glGenRenderbuffers(1, &RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
//
//
//	// Error checking framebuffer
//	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "Framebuffer error: " << fboStatus << std::endl;
//
//	// Create Frame Buffer Object
//
//	glGenFramebuffers(1, &postProcessingFBO);
//	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);
//
//	// Create Framebuffer Texture
//
//	glGenTextures(1, &postProcessingTexture);
//	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);
//
//	// Error checking framebuffer
//	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;
//
//
//	// Framebuffer for Shadow Map
//
//	glGenFramebuffers(1, &shadowMapFBO);
//
//	// Texture for Shadow Map FBO
//
//	unsigned int shadowMap;
//	glGenTextures(1, &shadowMap);
//	glBindTexture(GL_TEXTURE_2D, shadowMap);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//	// Prevents darkness outside the frustrum
//	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
//	// Needed since we don't touch the color buffer
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//
//	// Matrices needed for the light's perspective
//	float farPlane = 100.0f;
//	glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, farPlane);
//	glm::mat4 perspectiveProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
//	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//	glm::mat4 lightProjection = perspectiveProjection * lightView;
//
//	Shadow_Map_Program.Activate();
//	glUniformMatrix4fv(glGetUniformLocation(Shadow_Map_Program.Get_Program_Id(), "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
//
//
//	Main_Shader_Program.Activate();
//	glUniformMatrix4fv(glGetUniformLocation(Main_Shader_Program.Get_Program_Id(), "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
//	glUniform1f(glGetUniformLocation(Main_Shader_Program.Get_Program_Id(), "farPlane"), farPlane);
//
//
//	// Framebuffer for Cubemap Shadow Map
//
//	glGenFramebuffers(1, &pointShadowMapFBO);
//
//	// Texture for Cubemap Shadow Map FBO
//
//	glGenTextures(1, &depthCubemap);
//
//	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
//	for (unsigned int i = 0; i < 6; ++i)
//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
//			shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, pointShadowMapFBO);
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//
//	// Matrices needed for the light's perspective on all faces of the cubemap
//	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
//	glm::mat4 shadowTransforms[] =
//	{
//	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
//	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
//	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
//	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
//	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
//	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))
//	};
//	// Export all matrices to shader
//	Shadow_Cube_Map_Program.Activate();
//	glUniformMatrix4fv(glGetUniformLocation(Shadow_Cube_Map_Program.Get_Program_Id(), "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
//	glUniformMatrix4fv(glGetUniformLocation(Shadow_Cube_Map_Program.Get_Program_Id(), "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
//	glUniformMatrix4fv(glGetUniformLocation(Shadow_Cube_Map_Program.Get_Program_Id(), "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
//	glUniformMatrix4fv(glGetUniformLocation(Shadow_Cube_Map_Program.Get_Program_Id(), "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
//	glUniformMatrix4fv(glGetUniformLocation(Shadow_Cube_Map_Program.Get_Program_Id(), "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
//	glUniformMatrix4fv(glGetUniformLocation(Shadow_Cube_Map_Program.Get_Program_Id(), "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));
//	glUniform3f(glGetUniformLocation(Shadow_Cube_Map_Program.Get_Program_Id(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
//	glUniform1f(glGetUniformLocation(Shadow_Cube_Map_Program.Get_Program_Id(), "farPlane"), farPlane);
//
//}
//
////===================================================================================================
//// start program:
////===================================================================================================
//void AsEngine::Run()
//{
//	// Main while loop
//	while (!glfwWindowShouldClose(Window))
//	{
//		Get_FPS();
//
//		Update();
//
//		Render();
//
//		// Swap the back buffer with the front buffer
//		glfwSwapBuffers(Window);
//		// Take care of all GLFW events
//		glfwPollEvents();
//	}
//
//	//if main loop stop call destructor:
//	Destructor();
//}
//
////===================================================================================================
//// update & draw/render sections:
////===================================================================================================
//void AsEngine::Update()
//{
//	glEnable(GL_DEPTH_TEST);
//
//	// Preparations for the Shadow Map
//	glViewport(0, 0, shadowMapWidth, shadowMapHeight);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, pointShadowMapFBO);
//	glClear(GL_DEPTH_BUFFER_BIT);
//
//	Storage.Models[0]->Draw(Shadow_Map_Program, Camera);
//
//	// Switch back to the default framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	// Switch back to the default viewport
//	glViewport(0, 0, width, height);
//	// Bind the custom framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
//	// Specify the color of the background
//	glClearColor(pow(0.07f, gamma), pow(0.13f, gamma), pow(0.17f, gamma), 1.0f);
//	// Clean the back buffer and depth buffer
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	// Enable depth testing since it's disabled when drawing the framebuffer rectangle
//	glEnable(GL_DEPTH_TEST);
//
//	//-------------------------------------------------------
//	// Bind the custom framebuffer
//	// Specify the color of the background
//
//	//glClearColor(0.1f, 0.1f, 0.1f, 1.f);
//
//	// Clean the back buffer and depth buffer
//
//	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// Handles camera inputs
//	Camera.Inputs(Window);
//	// Updates and exports the camera matrix to the Vertex Shader
//	Camera.Update_Matrix(45.0f, 0.1f, 100.0f);
//
//
//	//Light1->Pos.x += 8e-2 * AsSettings::Delta_Time;
//	//Light2->Pos.z -= 8e-2 * AsSettings::Delta_Time;
//	//Light3->Pos.z += 8e-2 * AsSettings::Delta_Time;
//
//
//	//set camera positon into shaders
//	Update_Camera_Position();
//}
//
////===================================================================================================
//void AsEngine::Render()
//{
//	//Storage.Sky_Boxes[Current_Sky_Box_Ui]->Draw(Skybox_Shader_Program, Camera);
//
//	//Storage.Textures_Dif[0]->Bind();
//	//Storage.Textures_Spec[0]->Bind();
//	//Storage.Planes[0]->Draw(Test_Shader_Program, Camera);
//
//	//---------------------------------------------------------------------------
//	//Send the light matrix to the shader
//	glActiveTexture(GL_TEXTURE0 + 2);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
//	glUniform1i(glGetUniformLocation(Main_Shader_Program.Get_Program_Id(), "shadowCubeMap"), 2);
//
//
//	//----------------------------------------------------------------------------
//	Storage.Models[0]->Draw(Main_Shader_Program, Camera);
//
//	Storage.Lights[0]->Draw(Light_Shader_Program, Camera);
//
//	//Storage.Textures[0]->Bind();
//	//Storage.Boxes[0]->Draw(Test_Shader_Program_1, Camera);
//
//	//--------------------------------TEST
//	// Make it so the multisampling FBO is read while the post-processing FBO is drawn
//	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
//	// Conclude the multisampling and copy it to the post-processing FBO
//	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
//
//
//	// Bind the default framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	// Draw the framebuffer rectangle
//	Frame_Buffer_Program.Activate();
//	glBindVertexArray(rectVAO);
//	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
//	glActiveTexture(GL_TEXTURE0 + 0);
//	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//
//
////===================================================================================================
////===================================================================================================
//// private section:
////===================================================================================================
//void AsEngine::Get_FPS()
//{
//	// Updates counter and times
//	Crnt_Time = glfwGetTime();
//	Time_Diff = Crnt_Time - Prev_Time;
//	Frame_Counter++;
//
//	//get Delta time and set into the settings
//	double dt = (Time_Diff / (double)Frame_Counter);
//	AsSettings::Delta_Time = dt;
//
//	if (Time_Diff >= 1.0)
//	{
//		// Creates new title
//		std::string FPS = std::to_string((1.0 / Time_Diff) * Frame_Counter);
//		std::string ms = std::to_string(dt);
//		std::string newTitle = FPS + "FPS / " + ms + "s";
//		glfwSetWindowTitle(Window, newTitle.c_str());
//
//		// Resets times and counter
//		Prev_Time = Crnt_Time;
//		Frame_Counter = 0;
//
//		// Use this if you have disabled VSync
//		//camera.Inputs(window);
//	}
//}
//
////===================================================================================================
////In this function updated shader for camera position
//void AsEngine::Update_Camera_Position()
//{
//
//	for (Shader_Iterator = Shader_Programs.begin(); Shader_Iterator != Shader_Programs.end(); ++Shader_Iterator)
//	{
//		(*Shader_Iterator)->Activate();
//		glUniform3f(glGetUniformLocation((*Shader_Iterator)->Get_Program_Id(), "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);
//		// Export the camMatrix to the Vertex Shader of the pyramid
//		Camera.Matrix(*(*Shader_Iterator), "camMatrix");
//	}
//
//}
//
////===================================================================================================
//// destructor of current window:
////===================================================================================================
//void AsEngine::Destructor()
//{
//	//Delete all Shaders
//	for (Shader_Iterator = Shader_Programs.begin(); Shader_Iterator != Shader_Programs.end(); ++Shader_Iterator)
//		(*Shader_Iterator)->Delete();
//
//
//	//----!!! this is storage section code !!!! after testing must be delete !!!!!-----
//
//
//
//	//--------------end of storage section--------------------------------------------------------
//
//	// Delete window before ending the program
//	glfwDestroyWindow(Window);
//	// Terminate GLFW before ending the program
//	glfwTerminate();
//}
//
//
////====================================================================================================================================
////====================================================================================================================================
//
////====================================================================================================================================
////====================================================================================================================================
//
////====================================================================================================================================
//// point of start main function: -> start program
////====================================================================================================================================
//
////int main()
////{
////	AsEngine Engine;
////
////	Engine.Init();
////
////	Engine.Run();
////
////	return 0;
////}