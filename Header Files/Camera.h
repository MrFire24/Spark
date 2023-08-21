#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"


#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::vec3 Speed = glm::vec3(0.0f, 0.0f, 0.0f);

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float max_speed = 1.0f;
	float max_accs =  0.0001f;
	float accs_time = 10.0f;
	float sensitivity = 150.0f;
	float FOV = 65;
	

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);

	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);
	// Handles camera inputs
	void Inputs(GLFWwindow* window);

private:
	float isFlashlight_ = false;
	bool firstClick_ = true;
	bool isAnyButtonPressed_ = false;
	float motionStartTime = 0;

	float acceleration();
};