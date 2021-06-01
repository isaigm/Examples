#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdio>

int main()
{
    sf::RenderWindow window(sf::VideoMode(111, 90), "Nodders");
    int curr_frame = 0;
    float dt = 0;
    sf::Texture frames[141];
    sf::Sprite sprite;
    sf::Clock clock;
    for (int i = 0; i < 141; i++)
    {
        char path[16];
        snprintf(path, sizeof(path), "frames/%d.gif", i);
        frames[i].loadFromFile(path);
    }
    window.setFramerateLimit(60);
    window.setActive(true);
    while (window.isOpen())
    {
        dt += clock.restart().asSeconds();
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        if (dt >= (1.0f / 15.0f))
        {
            sprite.setTexture(frames[curr_frame]);
            if (++curr_frame > 140)
            {
                curr_frame = 0;
            }
            dt -= (1.0f / 15.0f);
        }
        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }
    return 0;
}
