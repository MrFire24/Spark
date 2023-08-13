#include "Camera.h"
#include <cmath>

#define Up glm::vec3(0.0f, 1.0f, 0.0f)

// ����������� ������ Camera
Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

// ���������� ������� ������
void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    // ������������� ������
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // �������� ������� ���� ��� ������
    view = glm::lookAt(Position, Position + Orientation, Up);

    // �������� ������� ��������
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // ���������� ������� ������
    cameraMatrix = projection * view;
}

// �������� ������� ������ � ������
void Camera::Matrix(Shader& shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

// ��������� ����� � ���������� � ����
void Camera::Inputs(GLFWwindow* window)
{
    bool isAnyButtonPressed = false;
    bool sprint;

    // ����������� ��������� ����
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        sprint = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    {
        sprint = false;
    }

    // ����������� "�����������" ������������
    float k = powf(1.0f / (powf(std::sqrt(Speed.x * Speed.x + Speed.y * Speed.y + Speed.z * Speed.z) - max_speed / 0.7, 2.0f) + 0.85f - (sprint ? 0.1 : 0)), 10) / 1000.0f;

    // ������������ ������� ������� W (�������� ������)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += Orientation * k;
    }

    // ������������ ������� ������� A (�������� �����)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += -glm::normalize(glm::cross(Orientation, Up)) * k;
    }

    // ������������ ������� ������� S (�������� �����)
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += -Orientation * k;
    }

    // ������������ ������� ������� D (�������� ������)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += glm::normalize(glm::cross(Orientation, Up)) * k;
    }

    // ������������ ������� ������� Space (�������� �����)
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += Up * k;
    }

    // ������������ ������� ������� Left Shift (�������� ����)
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += -Up * k;
    }

    // ��������� ��������
    Speed /= 1.1f;

    // ���������� �������
    Position += Speed;

    // ��������� ������� ������ ���� ��� ����
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        FOV -= (FOV - 25.0f) / 5.0f;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
    {
        FOV += (65.0f - FOV) / 5.0f;
    }

    // ��������� �������� ������ ��� ������� ����� ������ ����
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // ������� ��������� ����
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // ��������� ������� ������� ������
        if (firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        // ��������� ��������� ��������� ����
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // ������������ � ��������� ��������� ����
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        // ������ ����� ���������� ������������� �������
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        // ������������� ���������� � ������ �����
        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            Orientation = newOrientation;
        }

        // ������� ���������� ����� � ������
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // ��������� ��������� ���� � ����� ������
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // ����� ��������� ����
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}