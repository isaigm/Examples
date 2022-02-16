#include <SFML/Graphics.hpp>
#include <iostream>

int cnt = 0;
int timeSteps = 1000;
class Box
{
public:
    Box(float mass, float vel, float start_x, float start_y, sf::Color color, float size)
    {
        this->mass = mass;
        this->vel = vel;
        rect.setPosition(start_x, start_y);
        rect.setSize({size, size});
        rect.setFillColor(color);
    }
    void render(sf::RenderTarget &rt)
    {
        rt.draw(rect);
    }
    void check_wall_collision()
    {
        if (rect.getPosition().x < 10)
        {
            rect.setPosition({10, rect.getPosition().y});
            vel *= -1;
            cnt++;
        }
    }
    void check_collisions(Box &other)
    {
        if (rect.getGlobalBounds().intersects(other.rect.getGlobalBounds()))
        {
            float tVel = vel;
            vel = (vel * (mass - other.mass) + 2 * other.vel * other.mass) / (mass + other.mass);
            other.vel = (other.vel * (other.mass - mass) + 2 * tVel * mass) / (mass + other.mass);
            cnt++;
        }
    }
    void update(float dt)
    {
        rect.move(dt * vel, 0);
    }

private:
    float mass, vel;
    sf::RectangleShape rect;
};
int main()
{
    sf::Font font;
    font.loadFromFile("Minecraft.ttf");
    sf::Text text;
    text.setFont(font);
    text.setPosition(20, 10);
    sf::RenderWindow window(sf::VideoMode(800, 500), "Calculando pi a golpes");
    window.setVerticalSyncEnabled(true);
    sf::RectangleShape wall, floor;
    wall.setSize({10, 500});
    floor.setSize({800, 10});
    wall.setFillColor(sf::Color::White);
    floor.setFillColor(sf::Color::White);
    wall.setPosition(0, 0);
    floor.setPosition(0, 490);
    sf::Clock clock;
    Box box1(1, 0, 100, 390, sf::Color::Red, 100);
    Box box2(10000000000, -50.0f / timeSteps, 570, 290, sf::Color::Blue, 200);
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        char buff[64];
        snprintf(buff, 64, "Colisiones: %d", cnt);
        text.setString(buff);
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();
        }
        for (int i = 0; i < timeSteps; i++)

        {
            box1.update(dt);
            box2.update(dt);
            box1.check_wall_collision();
            box2.check_wall_collision();
            box1.check_collisions(box2);
        }

        window.clear();
        window.draw(wall);
        window.draw(floor);
        window.draw(text);
        box1.render(window);
        box2.render(window);
        window.display();
    }
    return 0;
}
