﻿#include "imgui.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <type_traits>
#include <filesystem>
#include <stb/stb_image.h>
#include <string>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "vec3.h"
#include "Object.h"
#include "Hitbox.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Model.h"

void rgb_to_hsv(float rgb[3], float hsv[3]) {
    float min = std::min(rgb[0], std::min(rgb[1], rgb[2]));
    float max = std::max(rgb[0], std::max(rgb[1], rgb[2]));
    float s = (max - min) / max;
    float rc = (max - rgb[0]) / (max - min);
    float gc = (max - rgb[1]) / (max - min);
    float bc = (max - rgb[2]) / (max - min);
    float h = 0;
    if (rgb[0] == max) h = 0.0 + bc - gc;
    else if (rgb[1] == max) h = 2.0 + rc - bc;
    else h = 4.0 + gc - rc;
    h = h / 6;
    if (h < 0) h = 1 + h;
    hsv[0] = h;
    hsv[1] = s;
    hsv[2] = max;
}

int main()
{
    // Инициализация GLFW
    glfwInit();

    // Указываем версию OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(1600, 800, "Test Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Не удалось создать окно GLFW";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Инициализация GLAD
    gladLoadGL();

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, 1600, 800);

    // Создаем шейдерную программу для основных объектов
    Shader shaderProgram("default");
    // Store mesh data in vectors for the mesh
    

    // Создаем шейдерную программу для источника света
    //Shader lightShader("light");
    // Store mesh data in vectors for the mesh


    

    // Настройки источника света
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);


    // Активация шейдера для источника света и передача параметров в шейдер
    //lightShader.Activate();
    //glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    //glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    // Активация шейдера для основных объектов и передача параметров в шейдер
    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    
    glEnable(GL_DEPTH_TEST);

    // Создаем камеру
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 3.0f));


    Model sword("sword");
    Model grindstone("Grindstone");

    sword.Scale /= 10;
    grindstone.Scale /= 20;

    sword.Position.x += 5;


    float a = 0;

    while (!glfwWindowShouldClose(window)) {
        
        if (a >= 5.0f) a = 0;
        a += 0.01;
        glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Обработка ввода и обновление камеры
        camera.Inputs(window);
        camera.updateMatrix(camera.FOV, 0.01f, 100.0f);

        //light.Draw(lightShader, camera);
        sword.Draw(shaderProgram, camera);
        grindstone.Draw(shaderProgram, camera);

        // Рендеринг источника света
        //lightShader.Activate();
        //camera.Matrix(lightShader, "camMatrix");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Освобождение ресурсов
    shaderProgram.Delete();
    //lightShader.Delete();
    

    glfwDestroyWindow(window);
    glfwTerminate();

    /*
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Window Title");
    ImGui::SFML::Init(window);

    bool circleExists = true;
    float circleRadius = 200.0f;
    int circleSegments = 75;
    float circleColor[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 };
    sf::CircleShape shape(circleRadius, circleSegments);
    shape.setFillColor(sf::Color
    (
        (int)(circleColor[0] * 255),
        (int)(circleColor[1] * 255),
        (int)(circleColor[2] * 255)
    )); // Color circle
    shape.setOrigin(circleRadius, circleRadius);
    shape.setPosition(window.getSize().x / 2, window.getSize().y / 2); // Center circle



    float rad1 = 150.0f, rad2 = 150.0f;
    float height1 = 10.f, height2 = 10.f;
    float RGB_Pos1[3] = { 0.5f, 0.25f, 0.25f };
    vec3f vec1 = vec3f( 0.5f, 0.f, 0.5f );
    float RGB_Pos2[3] = { 0.5f, 0.25f, 0.25f };
    vec3f vec2 = vec3f( 0.5f, 0.f, 0.5f );
    sf::CircleShape circ1(rad1, 100), circ2(rad2, 100);
    HitBox::Cylinder hitBox1(&vec1), hitBox2(&vec2);
    float hsv_1[3], hsv_2[3];

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        /*
        ImGui::Begin("Window title");
        ImGui::Text("Window Pos = %d, %d", window.getPosition().x, window.getPosition().y);
        ImGui::Checkbox("Circle", &circleExists);
        ImGui::SliderFloat("Radius", &circleRadius, 100.0f, 300.0f);
        ImGui::SliderInt("Sides", &circleSegments, 3, 100);
        ImGui::ColorEdit3("Color Circle", circleColor);
        ImGui::End();

        shape.setRadius(circleRadius);
        shape.setOrigin(circleRadius, circleRadius);
        shape.setPointCount(circleSegments);
        shape.setFillColor(sf::Color
        (
            (int)(circleColor[0] * 255),
            (int)(circleColor[1] * 255),
            (int)(circleColor[2] * 255)
        )); // Color circle
        /* /////////////


        ImGui::Begin("Circle 1");
        ImGui::SliderFloat("Radius", &rad1, 50.0f, 300.0f);
        ImGui::SliderFloat("Height", &height1, 10.0f, 100.0f);
        ImGui::ColorEdit3("Color Circle", RGB_Pos1);
        ImGui::Text("R: %f\nG: %f\nB: %f\n\n", RGB_Pos1[0], RGB_Pos1[1], RGB_Pos1[2]);
        rgb_to_hsv(RGB_Pos1, hsv_1);
        ImGui::Text("H: %f\nS: %f\nV: %f\n\n", hsv_1[0], hsv_1[1], hsv_1[2]);
        ImGui::Text("X: %f\nY: %f\nZ: %f\n", hsv_1[1] * window.getSize().x, hsv_1[0] * 100, (1. - hsv_1[2]) * window.getSize().y);
        ImGui::End(); 
        vec1 = vec3f(hsv_1[1] * window.getSize().x, hsv_1[0] * 100, (1. - hsv_1[2]) * window.getSize().y);

        ImGui::Begin("Circle 2");
        ImGui::SliderFloat("Radius", &rad2, 50.0f, 300.0f);
        ImGui::SliderFloat("Height", &height2, 10.0f, 100.0f);
        ImGui::ColorEdit3("Color Circle", RGB_Pos2);
        ImGui::Text("R: %f\nG: %f\nB: %f\n\n", RGB_Pos2[0], RGB_Pos2[1], RGB_Pos2[2]);
        rgb_to_hsv(RGB_Pos2, hsv_2);
        ImGui::Text("H: %f\nS: %f\nV: %f\n\n", hsv_2[0], hsv_2[1], hsv_2[2]);
        ImGui::Text("X: %f\nY: %f\nZ: %f\n", hsv_2[1] * window.getSize().x, hsv_2[0] * 100, (1. - hsv_2[2]) * window.getSize().y);
        ImGui::End();
        vec2 = vec3f(hsv_2[1] * window.getSize().x, hsv_2[0] * 100, (1. - hsv_2[2]) * window.getSize().y);

        circ1.setRadius(rad1);
        circ1.setOrigin(rad1, rad1);
        circ1.setPosition(vec1.x, vec1.z);
        hitBox1.Radius_ = rad1;
        hitBox1.Height_ = height1;

        circ2.setRadius(rad2);
        circ2.setOrigin(rad2, rad2);
        circ2.setPosition(vec2.x, vec2.z);
        hitBox2.Radius_ = rad2;
        hitBox2.Height_ = height2;


        if (hitBox1.isColliding(hitBox2)) circ1.setFillColor(sf::Color((1. - (vec1.y / 120.)) * 250., 0, 0));
        else circ1.setFillColor(sf::Color((1. - (vec1.y / 120.)) * 250., (1. - (vec1.y / 120.)) * 250., (1. - (vec1.y / 120.)) * 250.));
        if (hitBox2.isColliding(hitBox1)) circ2.setFillColor(sf::Color((1. - (vec1.y / 120.)) * 250., 0, 0));
        else circ2.setFillColor(sf::Color((1. - (vec2.y / 120.)) * 250., (1. - (vec2.y / 120.)) * 250., (1. - (vec2.y / 120.)) * 250.));


        window.clear(sf::Color(18, 33, 43)); // Color background
        //if (circleExists)
        //    window.draw(shape);
        if (vec1.y > vec2.y) {
            window.draw(circ2);
            window.draw(circ1);
        }
        else
        {
            window.draw(circ1);
            window.draw(circ2);
        }
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    */
    return 0;
}

//Оставь что-то в память Горшку