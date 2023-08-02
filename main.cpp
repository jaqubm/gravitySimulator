#include <cmath>
#include <vector>
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>

class GravitySource
{
    sf::Vector2f pos;
    float strength;
    sf::CircleShape circleShape;

public:
    GravitySource(float pos_x, float pos_y, float strength)
    {
        this->pos.x = pos_x - 25;
        this->pos.y = pos_y - 25;

        this->strength = strength;

        circleShape.setPosition(pos);
        circleShape.setFillColor(sf::Color::White);
        circleShape.setRadius(50);
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
};

class Particle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape circleShape;

public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y, sf::Color color)
    {
        this->pos.x = pos_x;
        this->pos.y = pos_y;

        this->vel.x = vel_x;
        this->vel.y = vel_y;

        circleShape.setPosition(pos);
        circleShape.setFillColor(color);
        circleShape.setRadius(5);
    }

    void render(sf::RenderWindow& window)
    {
        circleShape.setPosition(pos);
        window.draw(circleShape);
    }

    sf::Vector2f getPos()
    {
        return pos;
    }

    void updatePhysics(GravitySource& gravitySource, float deltaTime)
    {
        float distanceX = gravitySource.getPos().x - pos.x;
        float distanceY = gravitySource.getPos().y - pos.y;

        float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

        float inverseDistance = 1.f / distance;

        float normalizedX = inverseDistance * distanceX;
        float normalizedY = inverseDistance * distanceY;

        float inverseSquareDropoff = inverseDistance * inverseDistance;

        float accelerationX = normalizedX * gravitySource.getStrength() * inverseSquareDropoff * deltaTime;
        float accelerationY = normalizedY * gravitySource.getStrength() * inverseSquareDropoff * deltaTime;

        vel.x += accelerationX;
        vel.y += accelerationY;

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

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(1600, 1000), "Gravity Simulator", sf::Style::Close | sf::Style::Titlebar, settings);
    window.setFramerateLimit(120);

    std::cout << "Window Initialized" << std::endl;

    // Earth - 6e24, Moon - 7.3e22, Sun - 2e30

    std::vector<GravitySource> gravitySources;
    gravitySources.push_back(GravitySource(800, 500, 0x6e24));
    //gravitySources.push_back(GravitySource(500, 500, 500000));
    //gravitySources.push_back(GravitySource(1200, 500, 500000));

    int particlesNum = 20;
    std::vector<Particle> particles;

    for (int i=0; i<particlesNum; i++)
    {
        particles.push_back(Particle(randPosX(mt), randPosY(mt), randVel(mt), randVel(mt), sf::Color(randColor(mt), randColor(mt), randColor(mt))));
        //particles.push_back(Particle(600, 700, 4, (float)(0.2f + (0.1 / particlesNum) * i), sf::Color(dist(mt), dist(mt), dist(mt))));
    }

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        window.clear();

        for (auto & gravitySource : gravitySources)
        {
            for (auto & particle : particles)
            {
                particle.updatePhysics(gravitySource, deltaTime.asSeconds());
            }
        }

        for (auto & gravitySource : gravitySources) gravitySource.render(window);

        int count = 0;

        for (auto & particle : particles)
        {
            particle.render(window);

            if ((particle.getPos().x <= 1600 && particle.getPos().y <= 1000) || (particle.getPos().x >= 0 && particle.getPos().y >= 0)) count++;
        }

        //std::cout << "Currently visible particles on screen: " << count << std::endl;

        window.display();
    }

    return EXIT_SUCCESS;
}
