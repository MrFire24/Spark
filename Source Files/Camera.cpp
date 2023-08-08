#include "Camera.h"
#include <cmath>

#define Up glm::vec3(0.0f, 1.0f, 0.0f)

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
	bool isAnyButtonPressed = false;

	#define magic powf(1.0f / (powf(std::sqrt(Speed.x * Speed.x + Speed.y * Speed.y + Speed.z * Speed.z) - max_speed / 0.7, 2.0f) + 0.85f - (sprint ? 0.1 : 0)), 10) / 1000.0f

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		isAnyButtonPressed = true;
		//std::cout << std::endl << Speed.x << std::endl << Speed.y << std::endl << Speed.z << std::endl << std::sqrt(Speed.x * Speed.x + Speed.y * Speed.y + Speed.z * Speed.z) << std::endl << powf(1.0f / (powf(std::sqrt(Speed.x * Speed.x + Speed.y * Speed.y + Speed.z * Speed.z) - max_speed, 2.0f) + 1.0f), 100) << std::endl;
		Speed += Orientation * magic;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		isAnyButtonPressed = true;
		Speed += -glm::normalize(glm::cross(Orientation, Up)) * magic;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		isAnyButtonPressed = true;
		Speed += -Orientation * magic;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		isAnyButtonPressed = true;
		Speed += glm::normalize(glm::cross(Orientation, Up)) * magic;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		isAnyButtonPressed = true;
		Speed += Up * magic;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		isAnyButtonPressed = true;
		Speed += -Up * magic;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		sprint = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		sprint = false;
	}

	Speed /= 1.1f;

	Position += Speed;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		FOV -= (FOV - 30.0f) / 5.0f;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
	{
		FOV += (60.0f - FOV) / 5.0f;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

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
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}