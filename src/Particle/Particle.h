#pragma once

#include "include.h"

class Particle : public Object
{
    sf::Vector2f vel;

    //TODO: Delete when new collision is done !!!
    sf::Vertex line[2];

public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y, float radius, sf::Color color)
    {
        this->pos.x = pos_x;
        this->pos.y = pos_y;

        this->relPos.x = pos_x - radius;
        this->relPos.y = pos_y - radius;

        this->vel.x = vel_x;
        this->vel.y = vel_y;

        this->radius = radius;

        circleShape.setPosition(pos);
        circleShape.setFillColor(color);
        circleShape.setRadius(radius);
    }

    //TODO: Delete when new collision is done !!!
    void renderVelocity(sf::RenderWindow &window) const
    {
        window.draw(line, 2, sf::Lines);
    }

    bool checkCollision(GravitySource & gravitySource, float & deltaTime);

    void updatePosition(std::vector<GravitySource> & gravitySources, float deltaTime);

    void updatePhysics(std::vector<GravitySource> & gravitySources, float deltaTime);
};
