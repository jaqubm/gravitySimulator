#include "../include.h"

sf::Vector2f GravitySource::getPos()
{
    return pos;
}

float GravitySource::getStrength() const
{
    return strength;
}

float GravitySource::getRadius() const
{
    return radius;
}