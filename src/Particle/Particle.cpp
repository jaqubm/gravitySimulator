#include "include.h"

bool Particle::checkCollision(GravitySource & gravitySource, float & deltaTime)
{
    //Distance between GravitySource and Particle after updating position with velocity
    float distanceX = gravitySource.getPos().x - (pos.x + vel.x * deltaTime * 1000);
    float distanceY = gravitySource.getPos().y - (pos.y + vel.y * deltaTime * 1000);
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

    if (distance <= radius + gravitySource.getRadius())
    {
        //Movement before collision with gravitySource

        //Distance between GravitySource and Particle before Collision
        distanceX = gravitySource.getPos().x - pos.x;
        distanceY = gravitySource.getPos().y - pos.y;
        distance = std::sqrt(distanceX * distanceX + distanceY * distanceY) - radius - gravitySource.getRadius();

        //Calculating distance Particle needs to collide with gravitySource
        float vectorDistance = std::sqrt((vel.x * deltaTime * 1000) * (vel.x * deltaTime * 1000) + (vel.y * deltaTime * 1000) * (vel.y * deltaTime * 1000));
        float collisionMovementMultiplier = distance / vectorDistance;

        //Updating Position before Collision with deltaTime as milliSeconds
        pos.x += vel.x * deltaTime * 1000 * collisionMovementMultiplier;
        pos.y += vel.y * deltaTime * 1000 * collisionMovementMultiplier;


        //Movement after collision with gravitySource

        //Normal Vector
        distanceX = gravitySource.getPos().x - pos.x;
        distanceY = gravitySource.getPos().y - pos.y;

        //Distance between GravitySource and Particle in collision
        distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

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

        //Calculating distance Particle has left after collision with gravitySource
        collisionMovementMultiplier = 1.f - collisionMovementMultiplier;

        //Updating Position after Collision with deltaTime as milliSeconds
        pos.x += vel.x * deltaTime * 1000 * collisionMovementMultiplier;
        pos.y += vel.y * deltaTime * 1000 * collisionMovementMultiplier;

        return true;
    }

    return false;
}

void Particle::updatePosition(std::vector<GravitySource> & gravitySources, float deltaTime)
{
    //Detecting collision
    bool collision = false;
    for (auto &gravitySource : gravitySources)
    {
        if (checkCollision(gravitySource, deltaTime))
        {
            collision = true;
            break;
        }
    }

    //Updating Position with deltaTime as milliSeconds if there was no collision
    if (!collision)
    {
        pos.x += vel.x * deltaTime * 1000;
        pos.y += vel.y * deltaTime * 1000;
    }

    //Updating Render Position
    relPos.x = pos.x - radius;
    relPos.y = pos.y - radius;

    circleShape.setPosition(relPos);
}

void Particle::updatePhysics(std::vector<GravitySource> & gravitySources, float deltaTime)
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
