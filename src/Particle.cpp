#include "../include.h"

void Particle::updatePosition(std::vector<GravitySource>& gravitySources)
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

    //Updating Position
    pos.x += vel.x;
    pos.y += vel.y;

    //Updating Render Position
    relPos.x = pos.x - radius;
    relPos.y = pos.y - radius;

    circleShape.setPosition(relPos);
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
        float accelerationX = normalizedX * gravitySource.getStrength() * inverseSquareDropOff * deltaTime;
        float accelerationY = normalizedY * gravitySource.getStrength() * inverseSquareDropOff * deltaTime;

        //Updating Velocity
        vel.x += accelerationX;
        vel.y += accelerationY;
    }
}
