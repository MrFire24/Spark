#include "Camera.h"
#include <cmath>

#define Up glm::vec3(0.0f, 1.0f, 0.0f)

// Конструктор класса Camera
Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

// Обновление матрицы камеры
void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    // Инициализация матриц
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Создание матрицы вида для камеры
    view = glm::lookAt(Position, Position + Orientation, Up);

    // Создание матрицы проекции
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Обновление матрицы камеры
    cameraMatrix = projection * view;
}

// Передача матрицы камеры в шейдер
void Camera::Matrix(Shader& shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

// Обработка ввода с клавиатуры и мыши
void Camera::Inputs(GLFWwindow* window)
{
    bool isAnyButtonPressed = false;
    bool sprint;

    // Определение состояния бега
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        sprint = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    {
        sprint = false;
    }

    // Определение "магического" коэффициента
    float k = powf(1.0f / (powf(std::sqrt(Speed.x * Speed.x + Speed.y * Speed.y + Speed.z * Speed.z) - max_speed / 0.7, 2.0f) + 0.85f - (sprint ? 0.1 : 0)), 10) / 1000.0f;

    // Обрабатываем нажатие клавиши W (движение вперед)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += Orientation * k;
    }

    // Обрабатываем нажатие клавиши A (движение влево)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += -glm::normalize(glm::cross(Orientation, Up)) * k;
    }

    // Обрабатываем нажатие клавиши S (движение назад)
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += -Orientation * k;
    }

    // Обрабатываем нажатие клавиши D (движение вправо)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += glm::normalize(glm::cross(Orientation, Up)) * k;
    }

    // Обрабатываем нажатие клавиши Space (движение вверх)
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += Up * k;
    }

    // Обрабатываем нажатие клавиши Left Shift (движение вниз)
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        isAnyButtonPressed = true;
        Speed += -Up * k;
    }

    // Затухание скорости
    Speed /= 1.1f;

    // Обновление позиции
    Position += Speed;

    // Обработка средней кнопки мыши для зума
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        FOV -= (FOV - 25.0f) / 5.0f;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
    {
        FOV += (65.0f - FOV) / 5.0f;
    }

    // Обработка вращения камеры при нажатии левой кнопки мыши
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Скрытие указателя мыши
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Обработка первого нажатия кнопки
        if (firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        // Получение координат указателя мыши
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Нормализация и коррекция координат мыши
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        // Расчет новой ориентации вертикального взгляда
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        // Корректировка ориентации в случае крена
        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            Orientation = newOrientation;
        }

        // Поворот ориентации влево и вправо
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Установка указателя мыши в центр экрана
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Показ указателя мыши
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}