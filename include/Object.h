#pragma once

#include "../include.h"

class Object
{
protected:
    sf::Vector2f pos;
    sf::Vector2f relPos;
    float radius{};
    sf::CircleShape circleShape;

public:
    void render(sf::RenderWindow& window) const
    {
        window.draw(circleShape);
    }
};
