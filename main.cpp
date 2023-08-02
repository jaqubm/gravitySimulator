#include <cmath>
#include <vector>
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>

class GravitySource
{
    sf::Vector2f pos;
    sf::Vector2f relPos;
    float strength;
    float radius;
    sf::CircleShape circleShape;

public:
    GravitySource(float pos_x, float pos_y, float strength, float radius)
    {
        this->pos.x = pos_x;
        this->pos.y = pos_y;

        this->relPos.x = pos_x - radius;
        this->relPos.y = pos_y - radius;

        this->strength = strength;

        this->radius = radius;

        circleShape.setPosition(relPos);
        circleShape.setFillColor(sf::Color::White);
        circleShape.setRadius(radius);
    }

    void render(sf::RenderWindow& window)
    {
        window.draw(circleShape);
    }

    sf::Vector2f getPos()
    {
        return pos;
    }

    float getStrength() const
    {
        return strength;
    }

    float getRadius() const
    {
        return radius;
    }
};

class Particle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    float radius;
    sf::CircleShape circleShape;

public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y, float radius, sf::Color color)
    {
        this->pos.x = pos_x;
        this->pos.y = pos_y;

        this->vel.x = vel_x;
        this->vel.y = vel_y;

        this->radius = radius;

        circleShape.setPosition(pos);
        circleShape.setFillColor(color);
        circleShape.setRadius(radius);
    }

    void render(sf::RenderWindow& window)
    {
        circleShape.setPosition(pos);
        window.draw(circleShape);
    }

    void updatePhysics(GravitySource& gravitySource, float deltaTime)
    {
        float distanceX = gravitySource.getPos().x - pos.x;
        float distanceY = gravitySource.getPos().y - pos.y;

        float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

        float inverseDistance = 1.f / distance;

        float normalizedX = inverseDistance * distanceX;
        float normalizedY = inverseDistance * distanceY;

        float inverseSquareDropOff = inverseDistance * inverseDistance;

        float accelerationX = normalizedX * gravitySource.getStrength() * inverseSquareDropOff * deltaTime;
        float accelerationY = normalizedY * gravitySource.getStrength() * inverseSquareDropOff * deltaTime;

        vel.x += accelerationX;
        vel.y += accelerationY;

        if (distance < radius + gravitySource.getRadius())
        {
            std::cout << "Collision" << std::endl;
        }

        pos.x += vel.x;
        pos.y += vel.y;
    }
};

int main()
{
    //Using new random implemented in C++11
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randColor(0, 256);
    std::uniform_real_distribution<float> randPosX(0, 1600);
    std::uniform_real_distribution<float> randPosY(0, 1000);
    std::uniform_real_distribution<float> randVel(0, 1);

    //Setting up Antialiasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    //Creating and setting up window
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "Gravity Simulator", sf::Style::Close | sf::Style::Titlebar, settings);
    window.setFramerateLimit(120);

    //Creating deltaClock
    sf::Clock deltaClock;

    //Gravity strengths examples (not real ones)
    // Saturn - 0x7e22, Earth - 0x6e24, Moon - 0x2e30

    //Creating GravitySources
    std::vector<GravitySource> gravitySources;
    gravitySources.emplace_back(800, 500, 0x7e22, 150);   //Saturn
    //gravitySources.emplace_back(800, 500, 0x6e24, 70);    //Earth
    //gravitySources.emplace_back(800, 500, 0x2e30, 30);  //Moon

    //Creating Particles
    int particlesNum = 1;
    std::vector<Particle> particles;

    for (int i=0; i<particlesNum; i++)
    {
        //particles.emplace_back(randPosX(mt), randPosY(mt), randVel(mt), randVel(mt), sf::Color(randColor(mt), randColor(mt), randColor(mt)));
        particles.emplace_back(600, 800, (float)(0.2f + (0.1 / particlesNum) * i), (float)(0.2f + (0.1 / particlesNum) * i), 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));
        //particles.emplace_back(600, 800, 0, 0, 5, sf::Color(randColor(mt), randColor(mt), randColor(mt)));
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

        //Updating physics
        for (auto & gravitySource : gravitySources)
        {
            for (auto & particle : particles)
            {
                particle.updatePhysics(gravitySource, deltaTime.asSeconds());
            }
        }

        //Rendering GravitySources
        for (auto & gravitySource : gravitySources) gravitySource.render(window);

        //Rendering Particles
        for (auto & particle : particles) particle.render(window);

        //Displaying window
        window.display();
    }

    return EXIT_SUCCESS;
}
