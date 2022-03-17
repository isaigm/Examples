#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <stdlib.h>
#define WIDTH 800
#define HEIGHT 600
#define TOWER_WIDTH 20
#define N 3
class Block : public sf::RectangleShape {
public:
    Block(float width, float height) : sf::RectangleShape({ width, height }) {}
    void setRandomColor()
    {
        sf::Color color;
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
        setFillColor(color);
    }
    void setCenterAt(sf::Vector2f center)
    {
        float w = getSize().x;
        float h = getSize().y;
        setPosition({ center.x - w / 2, center.y - h / 2 });
    }
    int cameFrom = 0;
};
struct tower
{
public:
    tower(float x, float y)
    {
        mTower.setSize({ TOWER_WIDTH, 200 });
        mTower.setPosition(x, y);
        mTower.setFillColor(sf::Color::White);
        mNextBlockPos.x = mTower.getPosition().x + mTower.getSize().x / 2;
        mNextBlockPos.y = mTower.getPosition().y + mTower.getSize().y;
    }
    int getSize()
    {
        return mBlocks.size();
    }
    void render(sf::RenderTarget& rt)
    {
        rt.draw(mTower);
        for (auto& block : mBlocks)
        {
            rt.draw(block);
        }
    }
    void populate()
    {
        int width = 150;
        for (int i = 0; i < N; i++)
        {
            Block block(width, mBlockHeight );
            block.setRandomColor();
            block.setCenterAt(mNextBlockPos);
            mBlocks.push_back(block);
            width -= 15;
            mNextBlockPos.y -= mBlockHeight;
        }
    }
    int canPickBlock(sf::Vector2f mousePos)
    {
        for (size_t i = 0; i < mBlocks.size(); i++)
        {
            auto &block = mBlocks[i];
            if (block.getGlobalBounds().contains(mousePos.x, mousePos.y) && i == mBlocks.size() - 1)
            {
                return i;
            }
        }
        return -1;
    }
    Block *getBlock(int idx)
    {
        return &mBlocks[idx];
    }
    bool canPlaceBlock(Block &block)
    {
        if (mTower.getGlobalBounds().intersects(block.getGlobalBounds()))
        {
            if (mBlocks.size() == 0) return true;
            return block.getSize().x <= mBlocks.back().getSize().x;
        }
        return false;
    
    }
    void insertBlock(Block &block)
    {
        block.setCenterAt(mNextBlockPos);
        mNextBlockPos.y -= mBlockHeight;
        mBlocks.push_back(block);
    }
    void removeBlock()
    {
        if (mBlocks.size() > 0)
        {
            mBlocks.pop_back();
            mNextBlockPos.y += mBlockHeight;
        }
    }
    void restartLastBlockPos()
    {
        if (mBlocks.size() > 0)
        {
            mBlocks.back().setCenterAt({ mNextBlockPos.x, mNextBlockPos.y + mBlockHeight });
        }
    }
private:
    const float mBlockHeight = 25;
    std::vector<Block> mBlocks;
    sf::Vector2f mNextBlockPos;
    sf::RectangleShape mTower;
};

class Game {
public:
    Game() : mWindow(sf::VideoMode(WIDTH, HEIGHT), "Torres de Hanoi")
    {
        mWindow.setVerticalSyncEnabled(true);
        float x = (WIDTH - 3 * TOWER_WIDTH) / 4;
        mTowers.push_back(tower(x, HEIGHT / 2));
        mTowers.push_back(tower(TOWER_WIDTH + 2 * x, HEIGHT / 2));
        mTowers.push_back(tower(2 * TOWER_WIDTH + 3 * x, HEIGHT / 2));
        mTowers[mStartTower].populate();
    }
    void run() {
        while (mWindow.isOpen())
        {
            mMousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
            handleEvents();
            if (mSelectedBlock)
            {
                mSelectedBlock->setCenterAt(mMousePos);
          
            }
            if (mTowers[mEndTower].getSize() == N && !mGameOver)
            {
                std::cout << "BIEN HECHO\n";
                std::cout << "LO HICISTE EN " << mMoves << "\n";
                if (mMoves > ((1 << N) - 1))
                {

                    std::cout << "AUNQUE PUEDES MEJORAR :')\n";
                }
                else
                {
                    std::cout << "EL MINIMO NUMERO DE MOVIMIENTOS\n";
                }
                mGameOver = true;
            }
            render();
        }
    }
private:
    void handleEvents()
    {
        sf::Event ev;
        while (mWindow.pollEvent(ev)) {
            switch (ev.type)
            {
            case sf::Event::Closed:
                mWindow.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (!mSelectedBlock)
                {
                    pickBlock();
                }
                else
                {
                    placeBlock();
                    mMoves++;
                }
                break;
            default:
                break;
            }
            
        }
    }
    void render()
    {
        mWindow.clear(sf::Color::Black);
        for (auto& t : mTowers)
        {
            t.render(mWindow);
        }
        mWindow.display();
    }
    void pickBlock()
    {
        for (int i = 0; i < 3; i++)
        {
            int idx = mTowers[i].canPickBlock(mMousePos);
            if (idx != -1 && mSelectedBlock == nullptr)
            {
                mSelectedBlock = mTowers[i].getBlock(idx);
                break;
            }
        }
    }
    void placeBlock()
    {
        for (int i = 0; i < 3; i++)
        {
            if (mTowers[i].canPlaceBlock(*mSelectedBlock))
            {
                int cameFrom = mSelectedBlock->cameFrom;
                if (cameFrom == i)
                {
                    mTowers[i].restartLastBlockPos();
                }
                else
                {
                    mSelectedBlock->cameFrom = i;
                    mTowers[i].insertBlock(*mSelectedBlock);
                    mTowers[cameFrom].removeBlock();
                }
              
                mSelectedBlock = nullptr;
                break;
            }
        }
    }
    const int mStartTower = 0;
    const int mEndTower = 2;
    int mMoves = 0;
    bool mGameOver = false;
    sf::RenderWindow mWindow;
    Block* mSelectedBlock = nullptr;
    std::vector<tower> mTowers;
    sf::Vector2f mMousePos;
};

int main()
{
    Game game;
    game.run();
    return 0;
}
