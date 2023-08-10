#include "Scene.h"

Scene::Scene()
{
    //Using new random implemented in C++11
    std::random_device rd;
    mt = new std::mt19937(rd());

    //Setting up Antialiasing
    settings = new sf::ContextSettings();
    settings->antialiasingLevel = 8;

    //Creating and setting up window
    window = new sf::RenderWindow(sf::VideoMode(W, H), "Gravity Simulator", sf::Style::Close | sf::Style::Titlebar, *settings);
    window->setFramerateLimit(FPS);

    //Creating Background
    background = new sf::RectangleShape(sf::Vector2f(W, H));
    background->setFillColor(sf::Color(20, 25, 30));

    //Creating deltaCLock
    deltaClock = new sf::Clock();

    //Creating and loading up font
    font = new sf::Font();
    font->loadFromFile("res/arial.ttf");

    //Creating and setting up simulationText
    simulationText = new sf::Text();
    simulationText->setFont(*font);
    simulationText->setStyle(sf::Text::Bold);
    simulationText->setPosition(10, 10);
    simulationText->setCharacterSize(20);
    simulationText->setFillColor(sf::Color::Green);

    simulationText->setString(
    "jaqubm/gravity-simulator> Choose scene:\n\n"
    ">> TEST_SCENE\n"
    "   SCENE_0\n"
    "   SCENE_1\n"
    );

    sceneChooser = SceneChooser::TEST_SCENE;
    sceneState = SceneState::SIM_CHOOSE;
}

Scene::~Scene()
{
    delete window;
    delete background;
    delete mt;
}

void Scene::run()
{
    while (window->isOpen())
    {
        eventAction();

        window->clear();

        switch (sceneState)
        {
            case SceneState::SIM_CHOOSE:
            {
                sceneChooserRender();
                break;
            }
            case SceneState::SIM:
            {
                update();
                simulationRender();
                break;
            }
            default:
            {
                std::cout << "SceneState error" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        window->display();
    }
}

void Scene::update()
{
    //Updating framesCounter
    framesCounter++;

    //Calculating deltaTime
    sf::Time deltaTime = deltaClock->restart();

    //Calculating current fps
    fpsCounter = static_cast<int>(1.f / deltaTime.asSeconds());

    //Updating physics
    for (auto & particle : particles)
    {
        particle.updatePhysics(gravitySources, deltaTime.asSeconds());
        particle.updatePosition(gravitySources, deltaTime.asSeconds());
    }
}

void Scene::eventAction()
{
    sf::Event event{};
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) window->close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window->close();

        if (sceneState == SceneState::SIM_CHOOSE && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            sceneInit();
            sceneState = SceneState::SIM;
            deltaClock->restart();
            return;
        }

        if(sceneState == SceneState::SIM_CHOOSE && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            switch (sceneChooser)
            {
                case SceneChooser::TEST_SCENE:
                {
                    sceneChooser = SceneChooser::SCENE_1;
                    simulationText->setString(
                            "jaqubm/gravity-simulator> Choose scene:\n\n"
                            "   TEST_SCENE\n"
                            "   SCENE_0\n"
                            ">> SCENE_1\n"
                    );
                    break;
                }
                case SceneChooser::SCENE_0:
                {
                    sceneChooser = SceneChooser::TEST_SCENE;
                    simulationText->setString(
                            "jaqubm/gravity-simulator> Choose scene:\n\n"
                            ">> TEST_SCENE\n"
                            "   SCENE_0\n"
                            "   SCENE_1\n"
                    );
                    break;
                }
                case SceneChooser::SCENE_1:
                {
                    sceneChooser = SceneChooser::SCENE_0;
                    simulationText->setString(
                            "jaqubm/gravity-simulator> Choose scene:\n\n"
                            "   TEST_SCENE\n"
                            ">> SCENE_0\n"
                            "   SCENE_1\n"
                    );
                    break;
                }
                default:
                {
                    std::cout << "SceneChooser error" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        if(sceneState == SceneState::SIM_CHOOSE && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            switch (sceneChooser) {
                case SceneChooser::TEST_SCENE: {
                    sceneChooser = SceneChooser::SCENE_0;
                    simulationText->setString(
                            "jaqubm/gravity-simulator> Choose scene:\n\n"
                            "   TEST_SCENE\n"
                            ">> SCENE_0\n"
                            "   SCENE_1\n"
                    );
                    break;
                }
                case SceneChooser::SCENE_0: {
                    sceneChooser = SceneChooser::SCENE_1;
                    simulationText->setString(
                            "jaqubm/gravity-simulator> Choose scene:\n\n"
                            "   TEST_SCENE\n"
                            "   SCENE_0\n"
                            ">> SCENE_1\n"
                    );
                    break;
                }
                case SceneChooser::SCENE_1: {
                    sceneChooser = SceneChooser::TEST_SCENE;
                    simulationText->setString(
                            "jaqubm/gravity-simulator> Choose scene:\n\n"
                            ">> TEST_SCENE\n"
                            "   SCENE_0\n"
                            "   SCENE_1\n"
                    );
                    break;
                }
                default: {
                    std::cout << "SceneChooser error" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

void Scene::sceneInit() {
    switch (sceneChooser)
    {
        case SceneChooser::TEST_SCENE:
        {
            std::cout << "SceneChooser::TEST_SCENE" << std::endl;

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
                particles.emplace_back(W * .5f - 300, H * .5f + 200, .3f, static_cast<float>(.2f + (.1f / static_cast<float>(PARTICLE_NUM)) * static_cast<float>(i)), 5, sf::Color(randColor(*mt), randColor(*mt), randColor(*mt)));

            }

            break;
        }
        case SceneChooser::SCENE_0:
        {
            std::cout << "SceneChooser::SCENE_0" << std::endl;

            break;
        }
        case SceneChooser::SCENE_1:
        {
            std::cout << "SceneChooser::SCENE_1" << std::endl;

            break;
        }
        default:
        {
            std::cout << "SceneChooser error" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void Scene::sceneChooserRender()
{
    window->draw(*simulationText);
    window->draw(*background);

    window->draw(*simulationText);
}

void Scene::simulationRender()
{
    //Rendering Background
    window->draw(*background);

    //Rendering GravitySources
    for (auto & gravitySource : gravitySources) gravitySource.render(*window);

    //Rendering Particles
    for (auto & particle : particles) particle.render(*window);

    //Rendering simulationText
    simulationText->setString("FPS: " + std::to_string(fpsCounter) + "\nFrame: " + std::to_string(framesCounter));
    window->draw(*simulationText);
}