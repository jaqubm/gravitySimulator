#include "include.h"

void Particle::updatePosition(std::vector<GravitySource>& gravitySources, float deltaTime)
{
    //Collision with gravitySources
    for (auto &gravitySource : gravitySources)
    {
        //Normal Vector
        float distanceX = gravitySource.getPos().x - pos.x;
        float distanceY = gravitySource.getPos().y - pos.y;

        //Distance between GravitySource and Particle
        float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

        //Checking for collision
        if (distance <= radius + gravitySource.getRadius())
        {
            //Simplifying division to speed up calculations
            float inverseDistance = 1.f / distance;

            //Normalized Unit Vector
            float normalizedX = inverseDistance * distanceX;
            float normalizedY = inverseDistance * distanceY;

            //Projection Vector
            float projectionX = (vel.x * normalizedX + vel.y * normalizedY) * normalizedX;
            float projectionY = (vel.x * normalizedX + vel.y * normalizedY) * normalizedY;

            //Calculating new Velocity Vector
            vel.x = vel.x - 2 * projectionX;
            vel.y = vel.y - 2 * projectionY;
        }
    }

    //Updating Position with deltaTime as milliSeconds
    pos.x += vel.x * deltaTime * 1000;
    pos.y += vel.y * deltaTime * 1000;

    //Updating Render Position
    relPos.x = pos.x - radius;
    relPos.y = pos.y - radius;

    circleShape.setPosition(relPos);

    //TODO: Delete when new collision is done !!!
    line[0] = sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Red);
    line[1] = sf::Vertex(sf::Vector2f(pos.x + (vel.x * deltaTime * 1000), pos.y + (vel.y * deltaTime * 1000)), sf::Color::Red);
}

void Particle::updatePhysics(std::vector<GravitySource>& gravitySources, float deltaTime)
{
    for (auto &gravitySource : gravitySources)
    {
        //Normal Vector
        float distanceX = gravitySource.getPos().x - pos.x;
        float distanceY = gravitySource.getPos().y - pos.y;

        //Distance between GravitySource and Particle
        float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

        //Simplifying division to speed up calculations
        float inverseDistance = 1.f / distance;

        //Normalized Unit Vector
        float normalizedX = inverseDistance * distanceX;
        float normalizedY = inverseDistance * distanceY;

        float inverseSquareDropOff = inverseDistance * inverseDistance;

        //Calculating Acceleration
        float accelerationX = normalizedX * gravitySource.getStrength() * inverseSquareDropOff;
        float accelerationY = normalizedY * gravitySource.getStrength() * inverseSquareDropOff;

        //Updating Velocity with deltaTime as Seconds
        vel.x += accelerationX * deltaTime;
        vel.y += accelerationY * deltaTime;
    }
}
