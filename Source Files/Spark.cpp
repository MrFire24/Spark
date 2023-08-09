#include "imgui.h"
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
#include"Texture.h"
#include "ShaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Camera.h"


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

/*
GLfloat platform[] = {
     1.0f, 0.f,   0.f,      0.83f, 0.70f, 0.44f, 	 5.0f, 0.0f,      0.0f, 1.0f, 0.0f
    - 0.5f, 0.0f,  0.6f,     0.83f, 0.70f, 0.44f, 	 0.0f, 5.0f,      0.0f, 1.0f, 0.0f
    - 0.5f, 0.0f,  0.6f,     0.83f, 0.70f, 0.44f, 	 5.0f, 5.0f,      0.0f, 1.0f, 0.0f
};

GLuint indices[] =
{
    0, 1, 2
};
*/


GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2, // Bottom side
    0, 2, 3, // Bottom side
    4, 6, 5, // Left side
    7, 9, 8, // Non-facing side
    10, 12, 11, // Right side
    13, 15, 14 // Facing side
};


GLfloat lightVertices[] =
{ //     COORDINATES     //
    -0.1f, -0.1f,  0.1f,
    -0.1f, -0.1f, -0.1f,
     0.1f, -0.1f, -0.1f,
     0.1f, -0.1f,  0.1f,
    -0.1f,  0.1f,  0.1f,
    -0.1f,  0.1f, -0.1f,
     0.1f,  0.1f, -0.1f,
     0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};

int main()
{
    glfwInit();

    //Максимальная и минимальная версия GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    ///типа Core версия
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    GLFWwindow* window = glfwCreateWindow(1600, 800, "Test Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    int width, height; // не нравится
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, 1600, 800);
    ////

    // Создаем шейдер программу
    Shader shaderProgram("default.vert", "default.frag");
    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));
    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    Shader lightShader("light.vert", "light.frag");
    // Generates Vertex Array Object and binds it
    VAO lightVAO;
    lightVAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO lightVBO(lightVertices, sizeof(lightVertices));
    // Generates Element Buffer Object and links it to indices
    EBO lightEBO(lightIndices, sizeof(lightIndices));
    // Links VBO attributes such as coordinates and colors to VAO
    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    // Unbind all to prevent accidentally modifying them
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();

    glm::vec4 lightColor = glm::vec4(0.6f, 0.0f, 1.0f, 0.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);

    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    
    ////
    Texture wall("old_wall.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    wall.texUnit(shaderProgram, "tex0", 0);


    glEnable(GL_DEPTH_TEST);


    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 3.0f));

    float a = 0;


    ////
    while (!glfwWindowShouldClose(window)) {
        if (a >= 5.0f) a = 0;
        a += 0.01;
        glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        camera.Inputs(window);

        camera.updateMatrix(camera.FOV, 0.01f, 100.0f);
        lightColor = glm::vec4(a < 1.f ? a : (a < 2.f ? 1.f : (a < 3.f ? 3.f - a : 0.f)), a < 1.f ? 0.f : (a < 2.f ? a - 1.f : (a < 3.f ? 1.f : (a < 4.f ? 4.f - a : 0.f))), a < 2.f ? 0.f : (a < 3.f ? a - 2.f : (a < 4.f ? 1.f : (a < 5.f ? 5.f - a : 0.f))), 0.0f);

        shaderProgram.Activate();
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        camera.Matrix(shaderProgram, "camMatrix");

        wall.Bind();
        VAO1.Bind();

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);


        
        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix");

        lightVAO.Bind();
        glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    wall.Delete();
    shaderProgram.Delete();
    

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