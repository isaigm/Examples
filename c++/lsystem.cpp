#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <math.h>
#define SEGMENT_SIZE 1
#define WIDTH 800
#define HEIGHT 600
void drawKoch(sf::RenderTexture &rt)
{
    std::string currIter = "F";
    float angle = 0;
    std::map<char, std::string> rules;
    std::map<char, int> symbols;
    symbols['F'] = 0;
    symbols['-'] = -90;
    symbols['+'] = 90;
    rules['F'] = "F+F-F-F+F";
    float x = 30;
    float y = HEIGHT - 10;
    int maxIterations = 6;
    for (int i = 0; i < maxIterations; i++)

    {
        std::string nextIter;
        for (size_t idx = 0; idx < currIter.length(); idx++)
        {

            auto r = currIter[idx];
            if (rules.find(r) != rules.end())
            {
                nextIter += rules[r];
            }
            else
                nextIter.push_back(r);
        }

        currIter = nextIter;
    }

    rt.clear(sf::Color::White);
    for (auto ch : currIter)
    {
        if (ch == 'F')
        {
            float xx = x + SEGMENT_SIZE * cos(angle * M_PI / 180.0f);
            float yy = y + SEGMENT_SIZE * sin(angle * M_PI / 180.0f) * -1;
            sf::Vertex line[2];
            line[0].position = sf::Vector2f(x, y);
            line[0].color = sf::Color::Red;
            line[1].position = sf::Vector2f(xx, yy);
            line[1].color = sf::Color::Red;
            rt.draw(line, 2, sf::Lines);
            x = xx;
            y = yy;
        }
        angle += symbols[ch];
    }
    rt.display();
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Koch con sistemas L");
    window.setVerticalSyncEnabled(true);
    sf::Sprite sprite;
    sf::RenderTexture rt;
    rt.create(WIDTH, HEIGHT);

    drawKoch(rt);
   
    sprite.setTexture(rt.getTexture());
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

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }
    return 0;
}
