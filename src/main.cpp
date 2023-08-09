#include "../include/include.h"

//TODO: Create more advanced collision detection
//TODO: Create more advanced collision response

//TODO: Create Scene class to run the simulation

int main()
{
    //Using new random implemented in C++11
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randColor(0, 256);
    std::uniform_real_distribution<float> randPosX(0, W);
    std::uniform_real_distribution<float> randPosY(0, H);
    std::uniform_real_distribution<float> randVel(0, 1);

    //Setting up Antialiasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    //Creating and setting up window
    sf::RenderWindow window(sf::VideoMode(W, H), "Gravity Simulator", sf::Style::Close | sf::Style::Titlebar, settings);
    window.setFramerateLimit(FPS);

    //Creating deltaClock
    sf::Clock deltaClock;

    //Creating Background
    sf::RectangleShape background(sf::Vector2f(W, H));
    background.setFillColor(sf::Color(20, 25, 30));

    //Creating fpsCounter
    int fpsCounter;

    //Creating framesCounter
    long long framesCounter = 0;

    //Loading up font
    sf::Font font;
    font.loadFromFile("res/arial.ttf");

    //Creating and setting up simulationInfo
    sf::Text simulationInfo;
    simulationInfo.setFont(font);
    simulationInfo.setStyle(sf::Text::Bold);
    simulationInfo.setPosition(10, 10);
    simulationInfo.setCharacterSize(16);
    simulationInfo.setFillColor(sf::Color::White);

    //Gravity strengths examples (not real ones)
    // Saturn - 0x7e22, Earth - 0x6e24, Moon - 0x2e30

    //Creating GravitySources
    std::vector<GravitySource> gravitySources;

    //Testing examples
    //gravitySources.emplace_back(W * .5f, H * .5f, 0x7e22, 100);   //Saturn
    //gravitySources.emplace_back(W * .5f, H * .5f, 0x6e24, 60);    //Earth
    //gravitySources.emplace_back(W * .5f, H * .5f, 0x2e30, 10);  //Moon

    gravitySources.emplace_back(W * .5f - 300, H * .5f,  16000, 30);   //Saturn
    gravitySources.emplace_back(W * .5f + 300, H * .5f, 16000, 30);    //Earth

//    gravitySources.emplace_back(W * .5f - 300, H * .5f, 5 * 0x6e24, 30);   //Saturn
//    gravitySources.emplace_back(W * .5f + 300, H * .5f, 5 * 0x6e24, 30);    //Earth

    //Creating Particles
    int particlesNum = 1000;
    std::vector<Particle> particles;
    particles.reserve(particlesNum);

    for (int i=0; i<particlesNum; i++)
    {
        //Testing examples
        //particles.emplace_back(randPosX(mt), randPosY(mt), randVel(mt), randVel(mt), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));
        //particles.emplace_back(W * .5f - 300, H * .5f + 300, static_cast<float>(.2f + (.1f / static_cast<float>(particlesNum)) * static_cast<float>(i)), static_cast<float>(0.2f + (0.1 / particlesNum) * i), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));
        //particles.emplace_back(W * .5f - 300, H * .5f + 300, 2, static_cast<float>(.2f + (.1f / static_cast<float>(particlesNum)) * static_cast<float>(i)), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));
        particles.emplace_back(W * .5f - 300, H * .5f + 200, .3f, static_cast<float>(.2f + (.1f / static_cast<float>(particlesNum)) * static_cast<float>(i)), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));

    }

    //Main loop
    while (window.isOpen())
    {
        //Taking care of eventActions
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        //Clearing up window
        window.clear();

        //Updating framesCounter
        framesCounter++;

        //Calculating deltaTime
        sf::Time deltaTime = deltaClock.restart();

        //Calculating current fps
        fpsCounter = static_cast<int>(1.f / deltaTime.asSeconds());

        //Updating physics
        for (auto & particle : particles)
        {
            particle.updatePhysics(gravitySources, deltaTime.asSeconds());
            particle.updatePosition(gravitySources, deltaTime.asSeconds());
        }

        //Rendering Background
        window.draw(background);

        //Rendering GravitySources
        for (auto & gravitySource : gravitySources) gravitySource.render(window);

        //Rendering Particles
        for (auto & particle : particles) particle.render(window);

        //Rendering simulationInfo
        simulationInfo.setString("FPS: " + std::to_string(fpsCounter) + "\nFrame: " + std::to_string(framesCounter));
        window.draw(simulationInfo);

        //Displaying window
        window.display();
    }

    return EXIT_SUCCESS;
}
