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
    window.setFramerateLimit(static_cast<int>(FPS));

    //Creating deltaClock
    sf::Clock deltaClock;

    //Creating fpsCounter
    int fpsCounter = 0;

    //Loading up font
    sf::Font font;
    font.loadFromFile("res/arial.ttf");

    //Creating fpsText
    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setStyle(sf::Text::Bold);
    fpsText.setPosition(10, 10);
    fpsText.setCharacterSize(16);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setString("FPS: " + std::to_string(fpsCounter));

    //Gravity strengths examples (not real ones)
    // Saturn - 0x7e22, Earth - 0x6e24, Moon - 0x2e30

    //Creating GravitySources
    std::vector<GravitySource> gravitySources;

    //Testing examples
    //gravitySources.emplace_back(W * .5f, H * .5f, 0x7e22, 100);   //Saturn
    //gravitySources.emplace_back(W * .5f, H * .5f, 0x6e24, 60);    //Earth
    //gravitySources.emplace_back(W * .5f, H * .5f, 0x2e30, 10);  //Moon

    gravitySources.emplace_back(W * .5f - 300, H * .5f, 5 * 0x6e24, 30);   //Saturn
    gravitySources.emplace_back(W * .5f + 300, H * .5f, 5 * 0x6e24, 30);    //Earth

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
        particles.emplace_back(W * .5f - 300, H * .5f + 200, 2.8f, static_cast<float>(.1f + (.1f / static_cast<float>(particlesNum)) * static_cast<float>(i)), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));

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

        //Calculating deltaTime
        sf::Time deltaTime = deltaClock.restart();

        //Calculating current fps
        fpsCounter = static_cast<int>(1.f / deltaTime.asSeconds());

        //Updating physics
        for (auto & particle : particles)
        {
            particle.updatePhysics(gravitySources, deltaTime.asSeconds());
            particle.updatePosition(gravitySources);
        }

        //Rendering fpsText
        fpsText.setString("FPS: " + std::to_string(fpsCounter));
        window.draw(fpsText);

        //Rendering GravitySources
        for (auto & gravitySource : gravitySources) gravitySource.render(window);

        //Rendering Particles
        for (auto & particle : particles) particle.render(window);

        //Displaying window
        window.display();
    }

    return EXIT_SUCCESS;
}
