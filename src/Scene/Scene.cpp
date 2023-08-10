#include "Scene.h"

void Scene::mainLoop()
{
    while (window->isOpen())
    {
        //Taking care of eventActions
        sf::Event event{};
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window->close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window->close();
        }

        //Clearing up window
        window->clear();

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
        window->draw(*background);

        //Rendering GravitySources
        for (auto & gravitySource : gravitySources) gravitySource.render(*window);

        //Rendering Particles
        for (auto & particle : particles) particle.render(*window);

        //Rendering simulationInfo
        simulationInfo.setString("FPS: " + std::to_string(fpsCounter) + "\nFrame: " + std::to_string(framesCounter));
        window->draw(simulationInfo);

        //Displaying window
        window->display();
    }
}