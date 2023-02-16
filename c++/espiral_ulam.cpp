#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#define TILE_SIZE 5
#define WIDTH 800
#define HEIGHT 600
enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};
bool isPrime(int k)
{
    if (k == 1) return false;
    for (int i = 2; i * i <= k; i++)
    {
        if (k % i == 0) return false;
    }
    return true;
}
void spiral(sf::RenderTexture& rt)
{
    float x = float(WIDTH) / 2.0f;
    float y = float(HEIGHT) / 2.0f;
    int currLen = 1;
    int n = 2;
    int limit = WIDTH * HEIGHT / TILE_SIZE;
    Direction currDir = Direction::RIGHT;
    rt.clear(sf::Color::White);
    sf::RectangleShape rect({ TILE_SIZE, TILE_SIZE });
    rect.setFillColor(sf::Color::Black);
    rect.setPosition(x, y);
    rt.draw(rect);
    while (n <= limit)
    {
        
        for (int i = 0; i < currLen; i++, n++)
        {
            
            
            switch (currDir)
            {
            case Direction::LEFT:
                x -= TILE_SIZE;
                break;
            case Direction::RIGHT:
                x += TILE_SIZE;
                break;
            case Direction::UP:
                y -= TILE_SIZE;
                break;
            case Direction::DOWN:
                y += TILE_SIZE;
                break;
            }
          
            if (isPrime(n))
            {
                sf::RectangleShape rect({ TILE_SIZE, TILE_SIZE });
                rect.setPosition(x, y);
                rect.setFillColor(sf::Color::Blue);
                rt.draw(rect);
            }
        }
      
        switch (currDir)
        {
        case Direction::LEFT:
            currDir = Direction::DOWN;
            break;
        case Direction::RIGHT:
            currDir = Direction::UP;
            break;
        case Direction::UP:
            currDir = Direction::LEFT;
            currLen++;
            break;
        case Direction::DOWN:
            currDir = Direction::RIGHT;
            currLen++;
            break;
        }

    }


    rt.display();
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Espiral de Ulam");

    window.setVerticalSyncEnabled(true);
    sf::Sprite sprite;
    sf::RenderTexture rt;
    rt.create(WIDTH, HEIGHT);

    spiral(rt);
   
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

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
    return 0;
}

