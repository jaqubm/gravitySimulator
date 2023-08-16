#include "Scene.h"

Scene::Scene()
{
    //Using new random implemented in C++11
    mt = new std::mt19937(rd());
    randVel = std::uniform_real_distribution<float>(MIN_VEL, MAX_VEL);
    randPosX = std::uniform_real_distribution<float>(MIN_POS, MAX_POS);
    randPosY = std::uniform_real_distribution<float>(MIN_POS, MAX_POS);

    //Setting up Antialiasing
    settings = new sf::ContextSettings();
    settings->antialiasingLevel = 8;

    //Creating and setting up window
    window = new sf::RenderWindow(sf::VideoMode(W, H), "gravitySimulator by jaqubm", sf::Style::Close | sf::Style::Titlebar, *settings);
    window->setFramerateLimit(FPS);

    //Creating Background
    background = new sf::RectangleShape(sf::Vector2f(W, H));
    background->setFillColor(sf::Color(20, 25, 30));

    //Creating deltaCLock and deltaTime
    deltaClock = new sf::Clock();
    deltaTime = new sf::Time();

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
            "   SCENE_2\n"
            );

    //Creating and setting up simulationControls
    simulationControls = new sf::Text();
    simulationControls->setFont(*font);
    simulationControls->setStyle(sf::Text::Bold);
    simulationControls->setPosition(10, H - 30);
    simulationControls->setCharacterSize(10);
    simulationControls->setFillColor(sf::Color::White);

    simulationControls->setString(
            "Arrows (UP/DOWN) / Enter - Choose Scene\n"
            "ESC - Close gravitySimulator"
            );

    sceneChooser = SceneChooser::TEST_COLLISION;
    sceneState = SceneState::SIM_CHOOSE;
}

Scene::~Scene()
{
    delete window;
    delete settings;
    delete background;
    delete deltaClock;
    delete deltaTime;
    delete font;
    delete simulationText;
    delete simulationControls;
    delete mt;
}

void Scene::run()
{
    while (window->isOpen())
    {
        eventAction();

        window->clear();

        *deltaTime = deltaClock->restart();
        fpsCounter = static_cast<int>(1.f / deltaTime->asSeconds());

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
            case SceneState::SIM_PAUSE:
            {
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
    for (auto & particle : particles)
    {
        particle.updatePhysics(gravitySources, deltaTime->asSeconds());
        particle.updatePosition(gravitySources, deltaTime->asSeconds());
    }

    //Updating displayed fpsCounter and deltaTime
    simulationText->setString(
            "FPS: " + std::to_string(fpsCounter) +
            "\ndeltaTime: " + std::to_string(deltaTime->asSeconds() * 1000) + " ms"
    );
}

void Scene::eventAction()
{
    sf::Event event{};
    while (window->pollEvent(event))
    {
        //Closing window
        if (event.type == sf::Event::Closed) window->close();

        //Closing window with ESC button
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window->close();

        //Pausing Simulation
        if (sceneState == SceneState::SIM && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
        {
            sceneState = SceneState::SIM_PAUSE;
            return;
        }

        //Resuming paused Simulation
        if (sceneState == SceneState::SIM_PAUSE && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
        {
            sceneState = SceneState::SIM;
            return;
        }

        //Going back to SIM_CHOOSE state from Simulation
        if ((sceneState == SceneState::SIM || sceneState == SceneState::SIM_PAUSE) && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            window->setTitle("gravitySimulator by jaqubm");

            simulationText->setCharacterSize(20);
            simulationText->setFillColor(sf::Color::Green);

            simulationControls->setPosition(10, H - 30);
            simulationControls->setString(
                    "Arrows (UP/DOWN) / Enter - Choose Scene\n"
                    "ESC - Close gravitySimulator"
                    );

            sceneState = SceneState::SIM_CHOOSE;
            return;
        }

        //Initializing Simulation after choosing Scene
        if (sceneState == SceneState::SIM_CHOOSE && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            sceneInit();
            sceneState = SceneState::SIM;
            return;
        }

        //Changing currently selected Scene - UP
        if(sceneState == SceneState::SIM_CHOOSE && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            switch (sceneChooser)
            {
                case SceneChooser::TEST_COLLISION:
                {
                    sceneChooser = SceneChooser::SCENE_2;
                    break;
                }
                case SceneChooser::TEST_SCENE:
                {
                    sceneChooser = SceneChooser::TEST_COLLISION;
                    break;
                }
                case SceneChooser::SCENE_0:
                {
                    sceneChooser = SceneChooser::TEST_SCENE;
                    break;
                }
                case SceneChooser::SCENE_1:
                {
                    sceneChooser = SceneChooser::SCENE_0;
                    break;
                }
                case SceneChooser::SCENE_2:
                {
                    sceneChooser = SceneChooser::SCENE_1;
                    break;
                }
                default:
                {
                    std::cout << "SceneChooser error" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        //Changing currently selected Scene - DOWN
        if(sceneState == SceneState::SIM_CHOOSE && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            switch (sceneChooser) {
                case SceneChooser::TEST_COLLISION:
                {
                    sceneChooser = SceneChooser::TEST_SCENE;
                    break;
                }
                case SceneChooser::TEST_SCENE:
                {
                    sceneChooser = SceneChooser::SCENE_0;
                    break;
                }
                case SceneChooser::SCENE_0:
                {
                    sceneChooser = SceneChooser::SCENE_1;
                    break;
                }
                case SceneChooser::SCENE_1:
                {
                    sceneChooser = SceneChooser::SCENE_2;
                    break;
                }
                case SceneChooser::SCENE_2:
                {
                    sceneChooser = SceneChooser::TEST_COLLISION;
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
    gravitySources.clear();
    particles.clear();

    particles.reserve(PARTICLES_NUM);

    switch (sceneChooser)
    {
        case SceneChooser::TEST_COLLISION:  //Particles moving between two gravitySources
        {
            window->setTitle("gravitySimulator by jaqubm (TEST_SCENE)");

            gravitySources.emplace_back(W * .5f - 300, H * .5f, 144000, 70);
            gravitySources.emplace_back(W * .5f + 300, H * .5f, 144000, 70);

            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(W * .5f, H * .5f, randVel(*mt), 0, 5, sf::Color(randColor(*mt), randColor(*mt), randColor(*mt)));

            break;
        }
        case SceneChooser::TEST_SCENE:  //Randomized particles
        {
            window->setTitle("gravitySimulator by jaqubm (TEST_SCENE)");

            gravitySources.emplace_back(W * .5f, H * .5f, 144000, 70);
            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(randPosX(*mt), randPosY(*mt), randVel(*mt), randVel(*mt), 5, sf::Color(randColor(*mt), randColor(*mt), randColor(*mt)));

            break;
        }
        case SceneChooser::SCENE_0: //Particles orbiting around gravitySource
        {
            window->setTitle("gravitySimulator by jaqubm (SCENE_0)");

            gravitySources.emplace_back(W * .5f, H * .5f, 36000, 90);
            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(W * .5f - 200, H * .5f + 200, static_cast<float>(.2f + (.1f / static_cast<float>(PARTICLES_NUM)) * static_cast<float>(i)), static_cast<float>(0.2f + (0.1 / PARTICLES_NUM) * i), 5, sf::Color(randColor(*mt), randColor(*mt), randColor(*mt)));

            break;
        }
        case SceneChooser::SCENE_1: //Particles orbiting around gravitySources
        {
            window->setTitle("gravitySimulator by jaqubm (SCENE_1)");

            gravitySources.emplace_back(W * .5f - 300, H * .5f,  16000, 30);
            gravitySources.emplace_back(W * .5f + 300, H * .5f, 16000, 30);
            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(W * .5f - 300, H * .5f + 200, .3f, static_cast<float>(.2f + (.1f / static_cast<float>(PARTICLES_NUM)) * static_cast<float>(i)), 5, sf::Color(randColor(*mt), randColor(*mt), randColor(*mt)));

            break;
        }
        case SceneChooser::SCENE_2: //Particles creating fun shapes and interactions
        {
            window->setTitle("gravitySimulator by jaqubm (SCENE_2)");

            gravitySources.emplace_back(W * .5f - 300, H * .5f,  36000, 60);
            gravitySources.emplace_back(W * .5f + 300, H * .5f, 36000, 60);
            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(W * .5f, H * .5f + 250, .23f, static_cast<float>(.3f + (.1f / static_cast<float>(PARTICLES_NUM)) * static_cast<float>(i)), 5, sf::Color(randColor(*mt), randColor(*mt), randColor(*mt)));

            break;
        }
        default:
        {
            std::cout << "SceneChooser error" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    simulationText->setCharacterSize(12);
    simulationText->setFillColor(sf::Color::White);

    simulationControls->setPosition(10, H - 40);
    simulationControls->setString(
            "Space - Resume/Pause simulation\n"
            "Enter - Choose Scene\n"
            "ESC - Close gravitySimulator"
            );
}

void Scene::sceneChooserRender()
{
    switch (sceneChooser)
    {
        case SceneChooser::TEST_COLLISION:
        {
            simulationText->setString(
                    "jaqubm/gravity-simulator> Choose scene:\n\n"
                    ">> TEST_COLLISION\n"
                    "   TEST_SCENE\n"
                    "   SCENE_0\n"
                    "   SCENE_1\n"
                    "   SCENE_2\n"
                    );
            break;
        }
        case SceneChooser::TEST_SCENE:
        {
            simulationText->setString(
                    "jaqubm/gravity-simulator> Choose scene:\n\n"
                    "   TEST_COLLISION\n"
                    ">> TEST_SCENE\n"
                    "   SCENE_0\n"
                    "   SCENE_1\n"
                    "   SCENE_2\n"
                    );
            break;
        }
        case SceneChooser::SCENE_0:
        {
            simulationText->setString(
                    "jaqubm/gravity-simulator> Choose scene:\n\n"
                    "   TEST_COLLISION\n"
                    "   TEST_SCENE\n"
                    ">> SCENE_0\n"
                    "   SCENE_1\n"
                    "   SCENE_2\n"
                    );
            break;
        }
        case SceneChooser::SCENE_1:
        {
            simulationText->setString(
                    "jaqubm/gravity-simulator> Choose scene:\n\n"
                    "   TEST_COLLISION\n"
                    "   TEST_SCENE\n"
                    "   SCENE_0\n"
                    ">> SCENE_1\n"
                    "   SCENE_2\n"
                    );
            break;
        }
        case SceneChooser::SCENE_2:
        {
            simulationText->setString(
                    "jaqubm/gravity-simulator> Choose scene:\n\n"
                    "   TEST_COLLISION\n"
                    "   TEST_SCENE\n"
                    "   SCENE_0\n"
                    "   SCENE_1\n"
                    ">> SCENE_2\n"
                    );
            break;
        }
        default:
        {
            std::cout << "SceneChooser error" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    window->draw(*simulationText);
    window->draw(*background);

    window->draw(*simulationText);
    window->draw(*simulationControls);
}

void Scene::simulationRender()
{
    window->draw(*background);

    for (auto & gravitySource : gravitySources) gravitySource.render(*window);

    for (auto & particle : particles) particle.render(*window);

    window->draw(*simulationText);
    window->draw(*simulationControls);
}