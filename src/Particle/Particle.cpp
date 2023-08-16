#include <cmath>

#include "include.h"

bool Particle::checkCollision(GravitySource & gravitySource, float & deltaTime)
{
    sf::Vector2f diff {
            pos.x - gravitySource.getPos().x,
            pos.y - gravitySource.getPos().y
    };

    float a = vel.x * vel.x + vel.y * vel.y;
    float b = 2 * (diff.x * vel.x + diff.y * vel.y);
    float c = diff.x * diff.x + diff.y * diff.y - gravitySource.getRadius() * gravitySource.getRadius();

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false; // No collision

    float sqrtDiscriminant = std::sqrt(discriminant);
    float t1 = (-b + sqrtDiscriminant) / (2 * a);
    float t2 = (-b - sqrtDiscriminant) / (2 * a);

    //Calculating beforeCollisionMovementPercent
    float befColMovPercent = std::min(t1, t2) * .1f;

    if (befColMovPercent >= 0 && befColMovPercent <= 1)
    {
        //Calculating pointOfContact with gravitySource
        sf::Vector2f pointOfContact {
                pos.x + befColMovPercent * vel.x,
                pos.y + befColMovPercent * vel.y
        };

        //Updating Position to the pointOfContact with gravitySource
        pos.x = pointOfContact.x;
        pos.y = pointOfContact.y;

        //Normal Vector
        float distanceX = gravitySource.getPos().x - pointOfContact.x;
        float distanceY = gravitySource.getPos().y - pointOfContact.y;

        //Distance between GravitySource and Particle
        float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

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

        //Calculating afterCollisionMovementPercent
        float afterColMovPercent = 1.f - befColMovPercent;

        //Updating Position after Collision with deltaTime as milliSeconds
        pos.x += vel.x * afterColMovPercent * deltaTime * 1000;
        pos.y += vel.y * afterColMovPercent * deltaTime * 1000;

        return true;
    }

    return false;
}

void Particle::updatePosition(std::vector<GravitySource> & gravitySources, float deltaTime)
{
    bool collision = false;

    for (auto &gravitySource : gravitySources)
    {
        if (checkCollision(gravitySource, deltaTime))
        {
            collision = true;
            break;
        }
    }

    if (!collision)
    {
        //Updating Position with deltaTime as milliSeconds
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
