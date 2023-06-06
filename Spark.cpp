#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <type_traits>

#include "Backend.h"


int main()
{
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
    vec3f pos1 = vec3f(window.getSize().x / 2, 0, window.getSize().y / 2);
    vec3f pos2 = vec3f(window.getSize().x / 2, 0, window.getSize().y / 2);
    sf::CircleShape circ1(rad1, 100), circ2(rad2, 100);
    HitBox::Cylinder hitBox1(&pos1), hitBox2(&pos2);


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


        ImGui::Begin("Circle 1");
        ImGui::SliderFloat("Radius", &rad1, 50.0f, 300.0f);
        ImGui::SliderFloat("X", &pos1.x, 0.0f, window.getSize().x);
        ImGui::SliderFloat("Z", &pos1.z, 0.0f, window.getSize().y);
        ImGui::End();

        ImGui::Begin("Circle 2");
        ImGui::SliderFloat("Radius", &rad2, 50.0f, 300.0f);
        ImGui::SliderFloat("X", &pos2.x, 0.0f, window.getSize().x);
        ImGui::SliderFloat("Z", &pos2.z, 0.0f, window.getSize().y);
        ImGui::End();


        circ1.setRadius(rad1);
        circ1.setOrigin(rad1, rad1);
        circ1.setPosition(pos1.x, pos1.z);
        hitBox1.Radius_ = rad1;

        circ2.setRadius(rad2);
        circ2.setOrigin(rad2, rad2);
        circ2.setPosition(pos2.x, pos2.z);
        hitBox2.Radius_ = rad2;

        if (hitBox1.isColliding(hitBox2)) circ1.setFillColor(sf::Color(250, 0, 0));
        else circ1.setFillColor(sf::Color(250, 250, 250));
        if (hitBox2.isColliding(hitBox1)) circ2.setFillColor(sf::Color(250, 0, 0));
        else circ2.setFillColor(sf::Color(250, 250, 250));


        window.clear(sf::Color(18, 33, 43)); // Color background
        //if (circleExists)
        //    window.draw(shape);
        window.draw(circ1);
        window.draw(circ2);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}