#include "include.h"

bool Particle::checkCollision(GravitySource & gravitySource, float & deltaTime) {
    sf::Vector2f diff = { pos.x - gravitySource.getPos().x, pos.y - gravitySource.getPos().y };
    double a = vel.x * vel.x + vel.y * vel.y;
    double b = 2 * (diff.x * vel.x + diff.y * vel.y);
    double c = diff.x * diff.x + diff.y * diff.y - gravitySource.getRadius() * gravitySource.getRadius();

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false; // No collision
    }

    double sqrtDiscriminant = sqrt(discriminant);
    double t1 = (-b + sqrtDiscriminant) / (2 * a);
    double t2 = (-b - sqrtDiscriminant) / (2 * a);

    double t = std::min(t1, t2);

    float distanceX = gravitySource.getPos().x - pos.x + vel.x * deltaTime * 1000;
    float distanceY = gravitySource.getPos().y - pos.y + vel.y * deltaTime * 1000;

    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

    if ((t >= 0 && t <= 10)) {
        sf::Vector2f pointOfContact{};
        pointOfContact.x = pos.x + t * vel.x;
        pointOfContact.y = pos.y + t * vel.y;

        std::cout << "\nCollision\n";
        std::cout << pos.x << " " << pos.y << "\n";
        std::cout << pointOfContact.x << " " << pointOfContact.y << std::endl;

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

        return true;
    }
    else if (distance <= radius + gravitySource.getRadius())
    {
        std::cout << "\nCollision\n";
        std::cout << gravitySource.getRadius() << " " << distance << std::endl;

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

        //Updating Render Position
        relPos.x = pos.x - radius;
        relPos.y = pos.y - radius;

        circleShape.setPosition(relPos);
    }

    //TODO: Delete when new collision is done !!!
    line[0] = sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Red);
    line[1] = sf::Vertex(sf::Vector2f(pos.x + (vel.x * deltaTime * 1000), pos.y + (vel.y * deltaTime * 1000)), sf::Color::Red);
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
