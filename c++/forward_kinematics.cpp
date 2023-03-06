#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

#define RADIUS 50
#define WHEEL_RADIUS 10
#define CORE_RADIUS 5
#define WIDTH 800
#define HEIGHT 600
class Ray
{
public:
    Ray(sf::Vector2f start, sf::Vector2f end) : points(sf::LinesStrip, 2)
    {
        points[0].position = start;
        points[1].position = end;
        points[0].color = sf::Color::Red;
        points[1].color = sf::Color::Red;
    }
    Ray() : points(sf::LinesStrip, 2)
    {
        points[0].color = sf::Color::Red;
        points[1].color = sf::Color::Red;
    }
    void setStartPoint(sf::Vector2f point)
    {
        points[0].position = point;
    }
    void setEndPoint(sf::Vector2f point)
    {
        points[1].position = point;
    }
    void render(sf::RenderTarget &rt)
    {
        rt.draw(points);
    }

private:
    sf::VertexArray points;
};
class Robot
{
public:
    Robot(sf::Vector2f pos) : body(RADIUS), leftWheel(WHEEL_RADIUS), rightWheel(WHEEL_RADIUS), core(CORE_RADIUS), center(pos)
    {
        body.setOrigin({RADIUS, RADIUS});
        body.setPosition(pos);
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Blue);
        core.setOrigin({CORE_RADIUS, CORE_RADIUS});
        core.setPosition(center);
        core.setFillColor(sf::Color::Blue);
        body.setOutlineThickness(2.0f);
        leftWheel.setFillColor(sf::Color::White);
        leftWheel.setOrigin({WHEEL_RADIUS, WHEEL_RADIUS});
        rightWheel.setFillColor(sf::Color::White);
        rightWheel.setOrigin({WHEEL_RADIUS, WHEEL_RADIUS});
        currAngleWheels = robotAngle - M_PI / 2;
        updateWheelsPosition();
        updateRayPosition();
    }
    void render(sf::RenderTarget &rt)
    {
        rt.draw(body);
        rt.draw(leftWheel);
        rt.draw(rightWheel);
        rt.draw(core);
        ray.render(rt);
    }
    void update(float dt)
    {
        vx = WHEEL_RADIUS * cos(robotAngle) * (rotVelRight + rotVelLeft) / 2.0f;
        vy = WHEEL_RADIUS * sin(robotAngle) * (rotVelRight + rotVelLeft) / 2.0f;

        rotVel = float(WHEEL_RADIUS / (2.0f * RADIUS)) * (rotVelRight - rotVelLeft);
        robotAngle += dt * rotVel;
        currAngleWheels += dt * rotVel;
        center.y += dt * -vy;
        center.x += dt * vx;
        updateRayPosition();
        updateWheelsPosition();
        body.rotate(dt * rotVel);
        body.setPosition(center);
        core.setPosition(center);
    }
private:
    void updateWheelsPosition()
    {
        rightWheel.setPosition({center.x + RADIUS * cos(currAngleWheels), center.y - RADIUS * sin(currAngleWheels)});
        leftWheel.setPosition({center.x - RADIUS * cos(currAngleWheels), center.y + RADIUS * sin(currAngleWheels)});
    }
    void updateRayPosition()
    {
        ray.setStartPoint({center.x + RADIUS * cos(robotAngle), center.y + RADIUS * sin(-robotAngle)});
        ray.setEndPoint({center.x + rayLen * RADIUS * cos(robotAngle), center.y + rayLen * RADIUS * sin(-robotAngle)});
    }
    sf::CircleShape body;
    sf::CircleShape leftWheel;
    sf::CircleShape rightWheel;
    sf::CircleShape core;
    sf::Vector2f center;
    Ray ray;
    float rayLen = 1.5f;
    float rotVel = 0;
    float vx = 0;
    float vy = 0;
    float currAngleWheels = 0;
    // change this
    float rotVelLeft = 1;
    float rotVelRight = 1;
    float robotAngle = M_PI / 4;
};
int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "");
    window.setVerticalSyncEnabled(true);
    sf::Clock clock;
    Robot robot({WIDTH / 2.0f, HEIGHT / 2.0f});
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        robot.update(dt);
        window.clear();
        robot.render(window);
        window.display();
    }
    return 0;
}
