#pragma once

#include "include.h"

class GravitySource : public Object
{
    float strength;
    float radius;

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

    sf::Vector2f getPos();

    float getStrength() const;

    float getRadius() const;
};
