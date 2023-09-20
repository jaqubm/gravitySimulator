#include "Scene.h"

Scene::Scene()
{
    //Using new random implemented in C++11
    pMt = new std::mt19937(rd());
    randVel = std::uniform_real_distribution<float>(MIN_VEL, MAX_VEL);
    randPosX = std::uniform_real_distribution<float>(MIN_POS, MAX_POS);
    randPosY = std::uniform_real_distribution<float>(MIN_POS, MAX_POS);

    //Setting up Antialiasing
    pSettings = new sf::ContextSettings();
    pSettings->antialiasingLevel = 8;

    //Creating and setting up pWindow
    pWindow = new sf::RenderWindow(sf::VideoMode(W, H), "gravitySimulator by jaqubm", sf::Style::Close | sf::Style::Titlebar, *pSettings);
    pWindow->setFramerateLimit(FPS);

    //Creating Background
    pBackground = new sf::RectangleShape(sf::Vector2f(W, H));
    pBackground->setFillColor(sf::Color(20, 25, 30));

    //Creating deltaCLock and pDeltaTime
    pDeltaClock = new sf::Clock();
    pDeltaTime = new sf::Time();

    //Creating and loading up pFont
    pFont = new sf::Font();
    pFont->loadFromFile("res/arial.ttf");

    //Creating and setting up pSimulationText
    pSimulationText = new sf::Text();
    pSimulationText->setFont(*pFont);
    pSimulationText->setStyle(sf::Text::Bold);
    pSimulationText->setPosition(10, 10);
    pSimulationText->setCharacterSize(20);
    pSimulationText->setFillColor(sf::Color::Green);

    pSimulationText->setString(
            "jaqubm/gravity-simulator> Choose scene:\n\n"
            ">> TEST_SCENE\n"
            "   SCENE_0\n"
            "   SCENE_1\n"
            "   SCENE_2\n"
            );

    //Creating and setting up pSimulationControls
    pSimulationControls = new sf::Text();
    pSimulationControls->setFont(*pFont);
    pSimulationControls->setStyle(sf::Text::Bold);
    pSimulationControls->setPosition(10, H - 30);
    pSimulationControls->setCharacterSize(10);
    pSimulationControls->setFillColor(sf::Color::White);

    pSimulationControls->setString(
            "Arrows (UP/DOWN) / Enter - Choose Scene\n"
            "ESC - Close gravitySimulator"
            );

    sceneChooser = SceneChooser::TEST_COLLISION;
    sceneState = SceneState::SIM_CHOOSE;
}

Scene::~Scene()
{
    delete pWindow;
    delete pSettings;
    delete pBackground;
    delete pDeltaClock;
    delete pDeltaTime;
    delete pFont;
    delete pSimulationText;
    delete pSimulationControls;
    delete pMt;
}

void Scene::run()
{
    while (pWindow->isOpen())
    {
        eventAction();

        pWindow->clear();

        *pDeltaTime = pDeltaClock->restart();
        fpsCounter = static_cast<int>(1.f / pDeltaTime->asSeconds());

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

        pWindow->display();
    }
}

void Scene::update()
{
    //Updating Particles
    for (auto & particle : particles) particle.update(gravitySources, pDeltaTime->asSeconds());

    //Updating displayed fpsCounter and pDeltaTime
    pSimulationText->setString(
            "FPS: " + std::to_string(fpsCounter) +
            "\ndeltaTime: " + std::to_string(pDeltaTime->asSeconds() * 1000) + " ms"
    );
}

void Scene::eventAction()
{
    sf::Event event{};
    while (pWindow->pollEvent(event))
    {
        //Closing pWindow
        if (event.type == sf::Event::Closed) pWindow->close();

        //Closing pWindow with ESC button
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) pWindow->close();

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
            pWindow->setTitle("gravitySimulator by jaqubm");

            pSimulationText->setCharacterSize(20);
            pSimulationText->setFillColor(sf::Color::Green);

            pSimulationControls->setPosition(10, H - 30);
            pSimulationControls->setString(
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
            pWindow->setTitle("gravitySimulator by jaqubm (TEST_COLLISION)");

            gravitySources.emplace_back(W * .5f - 300, H * .5f, 144000, 70);
            gravitySources.emplace_back(W * .5f + 300, H * .5f, 144000, 70);

            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(W * .5f, H * .5f, randVel(*pMt), 0, 5, sf::Color(randColor(*pMt), randColor(*pMt), randColor(*pMt)));

            break;
        }
        case SceneChooser::TEST_SCENE:  //Randomized particles
        {
            pWindow->setTitle("gravitySimulator by jaqubm (TEST_SCENE)");

            gravitySources.emplace_back(W * .5f, H * .5f, 144000, 70);
            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(randPosX(*pMt), randPosY(*pMt), randVel(*pMt), randVel(*pMt), 5, sf::Color(randColor(*pMt), randColor(*pMt), randColor(*pMt)));

            break;
        }
        case SceneChooser::SCENE_0: //Particles orbiting around gravitySource
        {
            pWindow->setTitle("gravitySimulator by jaqubm (SCENE_0)");

            gravitySources.emplace_back(W * .5f, H * .5f, 36000, 90);
            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(W * .5f - 200, H * .5f + 200, static_cast<float>(.2f + (.1f / static_cast<float>(PARTICLES_NUM)) * static_cast<float>(i)), static_cast<float>(0.2f + (0.1 / PARTICLES_NUM) * i), 5, sf::Color(randColor(*pMt), randColor(*pMt), randColor(*pMt)));

            break;
        }
        case SceneChooser::SCENE_1: //Particles orbiting around gravitySources
        {
            pWindow->setTitle("gravitySimulator by jaqubm (SCENE_1)");

            gravitySources.emplace_back(W * .5f - 300, H * .5f,  16000, 30);
            gravitySources.emplace_back(W * .5f + 300, H * .5f, 16000, 30);
            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(W * .5f - 300, H * .5f + 200, .3f, static_cast<float>(.2f + (.1f / static_cast<float>(PARTICLES_NUM)) * static_cast<float>(i)), 5, sf::Color(randColor(*pMt), randColor(*pMt), randColor(*pMt)));

            break;
        }
        case SceneChooser::SCENE_2: //Particles creating fun shapes and interactions
        {
            pWindow->setTitle("gravitySimulator by jaqubm (SCENE_2)");

            gravitySources.emplace_back(W * .5f - 300, H * .5f,  36000, 60);
            gravitySources.emplace_back(W * .5f + 300, H * .5f, 36000, 60);
            for (int i=0; i < PARTICLES_NUM; i++) particles.emplace_back(W * .5f, H * .5f + 250, .23f, static_cast<float>(.3f + (.1f / static_cast<float>(PARTICLES_NUM)) * static_cast<float>(i)), 5, sf::Color(randColor(*pMt), randColor(*pMt), randColor(*pMt)));

            break;
        }
        default:
        {
            std::cout << "SceneChooser error" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    pSimulationText->setCharacterSize(12);
    pSimulationText->setFillColor(sf::Color::White);

    pSimulationControls->setPosition(10, H - 40);
    pSimulationControls->setString(
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
            pSimulationText->setString(
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
            pSimulationText->setString(
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
            pSimulationText->setString(
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
            pSimulationText->setString(
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
            pSimulationText->setString(
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
    pWindow->draw(*pSimulationText);
    pWindow->draw(*pBackground);

    pWindow->draw(*pSimulationText);
    pWindow->draw(*pSimulationControls);
}

void Scene::simulationRender()
{
    pWindow->draw(*pBackground);

    for (auto & gravitySource : gravitySources) gravitySource.render(*pWindow);

    for (auto & particle : particles) particle.render(*pWindow);

    pWindow->draw(*pSimulationText);
    pWindow->draw(*pSimulationControls);
}
