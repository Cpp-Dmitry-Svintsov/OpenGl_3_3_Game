#include "camera.h"

//====================================================================================================
// destructors and constructors section:
//====================================================================================================
ACamera::ACamera()
	:Width(AsSettings::Width), Height(AsSettings::Height)
{
	Position = glm::vec3(15.f, 15.f, 15.f);
	Orientation = glm::vec3(-1.f, -1.f, -1.f);
}

//====================================================================================================
// init section: !!!Use only if constructor has'nt arguments like ACamera(); !!!
//====================================================================================================
void ACamera::Init(glm::vec3 position)
{
	Position = position;
}

//====================================================================================================
// public section:
//====================================================================================================
void ACamera::Update_Matrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)Width / Height, nearPlane, farPlane);

	// Sets new camera matrix
	Camera_Matrix = projection * view;
}

void ACamera::Matrix(AShader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Camera_Matrix));
}



void ACamera::Inputs(GLFWwindow* window)
{
	float dt = AsSettings::Delta_Time;

	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += Speed * Orientation * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += Speed * -glm::normalize(glm::cross(Orientation, Up)) * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += Speed * -Orientation * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += Speed * glm::normalize(glm::cross(Orientation, Up)) * dt;
	}
	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	//{
	//	Position += Speed * Up * dt;
	//}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += Speed * -Up * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		Speed = Const_Speed * 2;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		Speed = Const_Speed;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (First_Click)
		{
			glfwSetCursorPos(window, (Width / 2), (Height / 2));
			First_Click = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = Sensitivity * (float)(mouseY - (Height / 2)) / Height;
		float rotY = Sensitivity * (float)(mouseX - (Width / 2)) / Width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (Width / 2), (Height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		First_Click = true;
	}
}