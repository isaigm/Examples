#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <stdlib.h>
using namespace std;
void setCenter(sf::RectangleShape &rect, float x, float y)
{
    float w = rect.getSize().x;
    float h = rect.getSize().y;
    rect.setPosition({x - w / 2, y - h / 2});
}
sf::Color getRandomColor()
{
    sf::Color color;
    color.r = rand() % 256;
    color.g = rand() % 256;
    color.b = rand() % 256;

    return color;
}
struct tower
{
public:
    tower(float x, float y)
    {
        mTower.setSize({20, 200});
        mTower.setPosition(x, y);
        mTower.setFillColor(sf::Color::White);
    }
    sf::RectangleShape mTower;
    std::vector<sf::RectangleShape> mBlocks;

    void render(sf::RenderTarget &rt)
    {
        rt.draw(mTower);
        for(auto &block: mBlocks)
        {
            rt.draw(block);
        }
    }
    void populate()
    {
        float x = mTower.getPosition().x + mTower.getSize().x / 2;
        float y = mTower.getPosition().y + mTower.getSize().y;
        float w = 100;
        float h = 25;
        for(int i = 0; i < 4; i++)
        {
            sf::RectangleShape block({w, h});
            block.setFillColor(getRandomColor());
            setCenter(block, x, y);
            mBlocks.push_back(block);
            w -= 15;
            y -= h;
        }
    }
    bool canSelect(sf::Vector2f mousePos)
    {
        for(size_t i = 0; i < mBlocks.size(); i++)
        {

            auto block = mBlocks[i];
            if(block.getGlobalBounds().contains(mousePos.x, mousePos.y) && i == mBlocks.size() - 1)
            {
                return true;
            }
        }
        return false;
    }
    bool canInsert(sf::RectangleShape &block)
    {
        return block.getSize().x < mBlocks.back().getSize().x;
    }
private:
    sf::Vector2f mStart;
};

class Game{
public:
    Game() : mWindow(sf::VideoMode(800, 600), "Torres de Hanoi")
    {
        mWindow.setVerticalSyncEnabled(true);
        mTowers.push_back(tower(100, 300));
        mTowers.push_back(tower(100 + 300, 300));
        mTowers.push_back(tower(100 + 600, 300));
        mTowers[0].populate();

    }
    void run(){
        while(mWindow.isOpen())
        {
            handleEvents();

            render();
        }
    }

private:
    void handleEvents()
    {
        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

        sf::Event ev;
        while(mWindow.pollEvent(ev)){
            if(ev.type == sf::Event::Closed)
            {
                mWindow.close();
                break;
            }else if(ev.type == sf::Event::MouseButtonPressed)
            {
                if(mTowers[0].canSelect(mousePos))
                {
                    std::cout << "sd\n";
                }
            }
        }
    }
    void render()
    {
        mWindow.clear(sf::Color::Black);
        for(auto &t: mTowers)
        {
            t.render(mWindow);
        }
        mWindow.display();
    }
    sf::RenderWindow mWindow;
    std::vector<tower> mTowers;
};

int main()
{
    Game game;
    game.run();
    return 0;
}
