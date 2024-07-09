#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H


#include "shader.h"

class ACamera
{
public:
	ACamera();

	void Init(glm::vec3 position);

	// Updates the camera matrix to the Vertex Shader
	void Update_Matrix(float FOVdeg, float nearPlane, float farPlane);
	// Exports the camera matrix to a shader
	void Matrix(AShader& shader, const char* uniform);
	// Handles camera inputs
	void Inputs(GLFWwindow* window);

	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 Camera_Matrix = glm::mat4(1.0f);
private:

	// Prevents the camera from jumping around when first clicking left click
	bool First_Click = true;

	// Stores the width and height of the window
	int Width;
	int Height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	const float Const_Speed = 2.f;
	float Speed = Const_Speed;
	float Sensitivity = 100.0f;
};
#endif