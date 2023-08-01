#include <cmath>
#include <vector>
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
        this->pos.x = pos_x;
        this->pos.y = pos_y;

        this->strength = strength;

        circleShape.setPosition(pos);
        circleShape.setFillColor(sf::Color::White);
        circleShape.setRadius(10);
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
    Particle(float pos_x, float pos_y, float vel_x, float vel_y)
    {
        this->pos.x = pos_x;
        this->pos.y = pos_y;

        this->vel.x = vel_x;
        this->vel.y = vel_y;

        circleShape.setPosition(pos);
        circleShape.setFillColor(sf::Color::White);
        circleShape.setRadius(5);
    }

    void render(sf::RenderWindow& window)
    {
        circleShape.setPosition(pos);
        window.draw(circleShape);
    }

    void setColor(sf::Color& color)
    {
        circleShape.setFillColor(color);
    }

    sf::Vector2f getPos()
    {
        return pos;
    }

    void updatePhysics(GravitySource& gravitySource)
    {
        float distanceX = gravitySource.getPos().x - pos.x;
        float distanceY = gravitySource.getPos().y - pos.y;

        float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

        float inverseDistance = 1.f / distance;

        float normalizedX = inverseDistance * distanceX;
        float normalizedY = inverseDistance * distanceY;

        float inverseSquareDropoff = inverseDistance * inverseDistance;

        float accelerationX = normalizedX * gravitySource.getStrength() * inverseSquareDropoff;
        float accelerationY = normalizedY * gravitySource.getStrength() * inverseSquareDropoff;

        vel.x += accelerationX;
        vel.y += accelerationY;

        pos.x += vel.x;
        pos.y += vel.y;
    }
};

sf::Color valToColor(float value)
{
    if (value < 0.0f) value = 0;
    if (value > 1.0f) value = 1;

    int r = 0, g, b = 0;

    if (value < 0.5f)
    {
        b = (int)(255 * (1.0f - 2 * value));
        g = (int)(255 * 2 * value);
    }
    else
    {
        g = (int)(255 * (1.0f - 2 * value));
        r = (int)(255 * 2 * value);
    }

    return sf::Color(r, g, b);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "Gravity Simulator");
    window.setFramerateLimit(120);

    std::cout << "Window Initialized" << std::endl;

    std::vector<GravitySource> gravitySources;
    gravitySources.push_back(GravitySource(500, 500, 7000));
    gravitySources.push_back(GravitySource(1200, 500, 7000));

    int particlesNum = 2000;

    std::vector<Particle> particles;

    for (int i=0; i<particlesNum; i++)
    {
        particles.push_back(Particle(600, 700, 4, (float)(0.2f + (0.1 / particlesNum) * i)));

        sf::Color color = valToColor((float)i / (float)particlesNum);

        particles[i].setColor(color);
    }

    while (window.isOpen())
    {
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
                particle.updatePhysics(gravitySource);
            }
        }

        for (auto & gravitySource : gravitySources) gravitySource.render(window);

        int count = 0;

        for (auto & particle : particles)
        {
            particle.render(window);

            if ((particle.getPos().x <= 1600 && particle.getPos().y <= 1000) || (particle.getPos().x >= 0 && particle.getPos().y >= 0)) count++;
        }

        std::cout << "Currently visible particles on screen: " << count << std::endl;

        window.display();
    }

    return EXIT_SUCCESS;
}
