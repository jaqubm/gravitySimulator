#pragma once

#include "include.h"

enum class SceneChooser {
    TEST_SCENE_0,
    SCENE_0
};

class Scene {
    sf::RenderWindow *window;
    sf::ContextSettings settings;

    sf::RectangleShape *background;

    sf::Clock deltaClock;

    sf::Font font;
    sf::Text simulationInfo;

    int fpsCounter = 0;
    long long framesCounter = 0;

    std::vector<GravitySource> gravitySources;
    std::vector<Particle> particles;

public:
    explicit Scene(const enum SceneChooser& sceneChooser)
    {
        //Using new random implemented in C++11
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> randColor;
        std::uniform_real_distribution<float> randVel;
        std::uniform_real_distribution<float> randPosX;
        std::uniform_real_distribution<float> randPosY;

        //Setting up Antialiasing
        settings.antialiasingLevel = 8;

        //Creating and setting up window
        window = new sf::RenderWindow(sf::VideoMode(W, H), "Gravity Simulator", sf::Style::Close | sf::Style::Titlebar, settings);
        window->setFramerateLimit(FPS);

        //Creating Background
        background = new sf::RectangleShape(sf::Vector2f(W, H));
        background->setFillColor(sf::Color(20, 25, 30));

        //Loading up font
        font.loadFromFile("res/arial.ttf");

        //Creating and setting up simulationInfo
        simulationInfo.setFont(font);
        simulationInfo.setStyle(sf::Text::Bold);
        simulationInfo.setFillColor(sf::Color::White);
        simulationInfo.setCharacterSize(20);
        simulationInfo.setPosition(10, 10);

        switch (sceneChooser)
        {
            case SceneChooser::TEST_SCENE_0:
            {
                std::cout << "SceneChooser::TEST_SCENE_1" << std::endl;

                //Gravity strengths examples (not real ones)
                // Saturn - 0x7e22, Earth - 0x6e24, Moon - 0x2e30

                //Testing examples
                //gravitySources.emplace_back(W * .5f, H * .5f, 0x7e22, 100);   //Saturn
                //gravitySources.emplace_back(W * .5f, H * .5f, 0x6e24, 60);    //Earth
                //gravitySources.emplace_back(W * .5f, H * .5f, 0x2e30, 10);  //Moon

                gravitySources.emplace_back(W * .5f - 300, H * .5f,  16000, 30);   //Saturn
                gravitySources.emplace_back(W * .5f + 300, H * .5f, 16000, 30);    //Earth

                //    gravitySources.emplace_back(W * .5f - 300, H * .5f, 5 * 0x6e24, 30);   //Saturn
                //    gravitySources.emplace_back(W * .5f + 300, H * .5f, 5 * 0x6e24, 30);    //Earth

                //Creating Particles
                particles.reserve(PARTICLE_NUM);

                for (int i=0; i<PARTICLE_NUM; i++)
                {
                    //Testing examples
                    //particles.emplace_back(randPosX(mt), randPosY(mt), randVel(mt), randVel(mt), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));
                    //particles.emplace_back(W * .5f - 300, H * .5f + 300, static_cast<float>(.2f + (.1f / static_cast<float>(particlesNum)) * static_cast<float>(i)), static_cast<float>(0.2f + (0.1 / particlesNum) * i), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));
                    //particles.emplace_back(W * .5f - 300, H * .5f + 300, 2, static_cast<float>(.2f + (.1f / static_cast<float>(particlesNum)) * static_cast<float>(i)), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));
                    particles.emplace_back(W * .5f - 300, H * .5f + 200, .335f, static_cast<float>(.2f + (.1f / static_cast<float>(PARTICLE_NUM)) * static_cast<float>(i)), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));

                }

                break;
            }
            case SceneChooser::SCENE_0:
            {
                std::cout << "SceneChooser::SCENE_0" << std::endl;

                break;
            }
            default:
            {
                std::cout << "SceneChooser error" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    ~Scene()
    {
        delete window;
        delete background;
    }

    void mainLoop();
};
