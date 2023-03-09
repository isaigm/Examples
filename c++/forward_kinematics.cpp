#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#define WIDTH 800
#define HEIGHT 600
#define RADIUS 25
#define WHEEL_RADIUS 10
#define CORE_RADIUS 5
#define M_PI 3.14159f

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
    sf::Vector2f getEndPoint()
    {
        return points[1].position;
    }
    void setStartPoint(sf::Vector2f point)
    {
        points[0].position = point;
    }
    void setEndPoint(sf::Vector2f point)
    {
        points[1].position = point;
    }
    void render(sf::RenderTarget& rt)
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
        body.setOrigin({ RADIUS, RADIUS });
        body.setPosition(pos);
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Blue);
        core.setOrigin({ CORE_RADIUS, CORE_RADIUS });
        core.setPosition(center);
        core.setFillColor(sf::Color::Blue);
        body.setOutlineThickness(2.0f);
        leftWheel.setFillColor(sf::Color::White);
        leftWheel.setOrigin({ WHEEL_RADIUS, WHEEL_RADIUS });
        rightWheel.setFillColor(sf::Color::White);
        rightWheel.setOrigin({ WHEEL_RADIUS, WHEEL_RADIUS });
        currAngleWheels = robotAngle - M_PI / 2;
        updateWheelsPosition();
        updateRayPosition();
    }
    void goTo(sf::Vector2f target)
    {
        float dx = target.x - center.x;
        float tTarget = (HEIGHT - target.y);
        float tCenter = (HEIGHT - center.y);
        float dy = tTarget - tCenter;
        float dist = sqrt(dx * dx + dy * dy);
        sf::Vector2f v1 = { target.x - center.x, target.y - center.y };
        sf::Vector2f v2 = { ray.getEndPoint().x - center.x, ray.getEndPoint().y - center.y };
        float dotProduct = v1.x * v2.x + v1.y * v2.y;
        float lenV1 = sqrt(v1.x * v1.x + v1.y * v1.y);
        float lenV2 = sqrt(v2.x * v2.x + v2.y * v2.y);
        float angleBetween = acos(dotProduct / (lenV1 * lenV2));
        lastTarget = target;
        float f1 = robotAngle + angleBetween; //para determinar en que sentido debe girar el robot
        float f2 = robotAngle - angleBetween;
        float m = dy / dx;
        float dir = atan(m);
        if (dist < 5)
        {
            stop();
            return;
        }
        if (target.x < center.x)
        {
            dir += M_PI;
        }
        if (dir < 0)
        {
            dir += 2 * M_PI;
        }
        if (f1 > 2 * M_PI)
            f1 -= 2 * M_PI;
        if (f2 < 0)
            f2 += 2 * M_PI;
        float vv1 = std::abs(f1 - dir);
        float vv2 = std::abs(f2 - dir);
        if (vv2 < vv1)
        {
            clockwise();
        }
        else
            anticlockwise();
        if (angleBetween < 0.06f)
        {
            forward();
        }
    }
    void render(sf::RenderTarget& rt)
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
        if (robotAngle < 0)
            robotAngle += 2 * M_PI;
        if (robotAngle > 2 * M_PI)
            robotAngle = 0;
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
    void forward()
    {
        rotVelLeft = 6;
        rotVelRight = 6;
    }
    void clockwise()
    {
        rotVelLeft = 5;
        rotVelRight = -5;
    }
    void stop()
    {
        rotVelLeft = 0;
        rotVelRight = 0;
    }
    void anticlockwise()
    {
        rotVelLeft = -5;
        rotVelRight = 5;
    }
    void updateWheelsPosition()
    {
        rightWheel.setPosition({ center.x + RADIUS * cos(currAngleWheels), center.y - RADIUS * sin(currAngleWheels) });
        leftWheel.setPosition({ center.x - RADIUS * cos(currAngleWheels), center.y + RADIUS * sin(currAngleWheels) });
    }
    void updateRayPosition()
    {
        ray.setStartPoint({ center.x + RADIUS * cos(robotAngle), center.y + RADIUS * sin(-robotAngle) });
        ray.setEndPoint({ center.x + rayLen * RADIUS * cos(robotAngle), center.y + rayLen * RADIUS * sin(-robotAngle) });
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
    sf::Vector2f lastTarget;
    // change this
    float rotVelLeft = 0;
    float rotVelRight = 0;
    float robotAngle = M_PI / 4;

};
int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "");
    window.setVerticalSyncEnabled(true);
    sf::Clock clock;
    Robot robot({ WIDTH / 2.0f - 100, HEIGHT / 2.0f });
    sf::Vector2f mousePos;
    bool canGo = false;
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
            else if (ev.type == sf::Event::MouseButtonPressed)
            {
                mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
                canGo = true;
            }
        }
        robot.update(dt);
        if (canGo)
            robot.goTo(mousePos);
        window.clear();
        robot.render(window);
        window.display();
    }
    return 0;
}
