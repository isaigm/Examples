#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#define WIDTH 150
#define HEIGHT 100
#define R 30.0f
class Eye
{
public:
    Eye(float xx, float yy, float r)
    {
        x = xx;
        y = yy;
        radius = r;
        eye.setFillColor(sf::Color::Transparent);
        eye.setOutlineColor(sf::Color::Black);
        eye.setOutlineThickness(5.0f);
        eye.setRadius(r);
        eye.setOrigin({r, r});
        eye.setPosition(x, y);

        pupil.setFillColor(sf::Color::Black);
        pupil.setRadius(10.0f);
        pupil.setOrigin({10.0f, 10.0f});
        pupil.setPosition(x, y);
    }
    void update(sf::Vector2i mousePos)
    {
        float dy = float(mousePos.y) - y;
        float dx = float(mousePos.x) - x;

        float alpha = atan(dy / dx);
        if (dx < 0)
            alpha += M_PI;
        if (sqrt(dx * dx + dy * dy) < eye.getRadius())
        {
            pupil.setPosition(sf::Vector2f(mousePos));
        }
        else
        {
            pupil.setPosition(x + (radius - 15) * cos(alpha), y + (radius - 15) * sin(alpha));
        }
    }
    void draw(sf::RenderTarget &target)
    {
        target.draw(eye);
        target.draw(pupil);
    }

private:
    sf::CircleShape eye;
    sf::CircleShape pupil;
    float x, y, radius;
};
int main()
{
    float space = 15; // espacio entre los ojos
    float sep = (WIDTH - 4 * R - space) / 2;
    Eye leftEye(sep + R, HEIGHT / 2, R);
    Eye rightEye(sep + 3 * R + space, HEIGHT / 2, R);
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "sfEyes");
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        leftEye.update(sf::Mouse::getPosition(window));
        rightEye.update(sf::Mouse::getPosition(window));
        window.clear(sf::Color::White);
        leftEye.draw(window);
        rightEye.draw(window);
        window.display();
    }
    return 0;
}
