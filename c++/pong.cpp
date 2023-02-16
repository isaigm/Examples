#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#define WIDTH 800
#define HEIGHT 600
#define BALL_R 10
#define PI 3.1459f
#define PADDLE_WITDH 10
#define PADDLE_HEIGHT 100
#define CPU 0
#define HUMAN 1
bool mustUpdateCpu = false;
static auto DEFAULT_COLOR = sf::Color::Green;
float degToRad(float deg)
{
    return PI * deg / 180.0f;
}
int getRandIn(int min, int max)
{
    return rand() % (max - min) + min;
}
class Paddle
{
public:
    Paddle() : mRect({ PADDLE_WITDH, PADDLE_HEIGHT})
    {
        mRect.setFillColor(DEFAULT_COLOR);
    }
    void setPos(float x, float y)
    {
        mCpuypos = y + PADDLE_HEIGHT / 2;
        mRect.setPosition(x, y);
    }
    void render(sf::RenderTarget& rt)
    {
        rt.draw(mRect);
    }
    void update(float dt)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            mRect.move(0, -speed * dt);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            mRect.move(0, speed * dt);
        }
    }
    void updateCpu(float dt, float ypos)
    {
        if (mustUpdateCpu)
        {
            if (std::abs(mCpuypos - ypos) < 5.0f) {}
            else if (mCpuypos > ypos)
            {
                mCpuypos -= dt * speed;
            }
            else if (mCpuypos < ypos)
            {
                mCpuypos += dt * speed;
            }
            mRect.setPosition(getPos().x, mCpuypos - PADDLE_HEIGHT / 2);
        }
    }
    sf::Vector2f getPos()
    {
        return mRect.getPosition();
    }
    const sf::RectangleShape& getRect()
    {
        return mRect;
    }
private:
    sf::RectangleShape mRect; 
    float speed = 300;
    float mCpuypos;
   
};
class Ball {
public:
    Ball()
    {
        mBall.setRadius(BALL_R);
        mBall.setFillColor(sf::Color::White);
        mBall.setOrigin({ BALL_R, BALL_R });
        mDir = getRandIn(45, 135);
    }
    void render(sf::RenderTarget& rt)
    {
        rt.draw(mBall);
    }
    void setPos(float x, float y)
    {
        mPos = mStartPos = { x, y };
        mBall.setPosition(mPos);
    }
    void update(float dt)
    {
        mPos.x += mSpeed * dt * sin(degToRad(mDir));
        mPos.y += mSpeed * dt * cos(degToRad(mDir));
        if (mPos.y < 0 || mPos.y > HEIGHT)
        {
            mDir = 180 - mDir;
            mPos.x += mSpeed * dt * sin(degToRad(mDir));
            mPos.y += mSpeed * dt * cos(degToRad(mDir));
        }
        if (mPos.x < 0 || mPos.x > WIDTH)
        {
            mDir = getRandIn(45, 135);
            mSpeed = -300;
            mPos = mStartPos;
        }
        mBall.setPosition(mPos);
    }
    void collidesWith(Paddle& paddle, int player)
    {
        bool collision = mBall.getGlobalBounds().intersects(paddle.getRect().getGlobalBounds());
        if (collision)
        {
            mDir = getRandIn(45, 135);
            mSpeed *= -1;
            auto pos = paddle.getPos();
            if (player == CPU)
            {
                mPos.x = pos.x - BALL_R;
                mustUpdateCpu = false;
            }
            else if (player == HUMAN)
            {
                mPos.x = pos.x + PADDLE_WITDH + BALL_R;
                mustUpdateCpu = true;
            }
        }
    }
    sf::Vector2f getPos()
    {
        return mBall.getPosition();
    }
private:
    sf::CircleShape mBall;
    float mDir;
    float mSpeed = -300;
    sf::Vector2f mPos;
    sf::Vector2f mStartPos;
};
class Background
{
public:
    Background()
    {
        float rectWidth = 10;
        int nrects = 15;
        float ysep = 10;
        float rectHeight = (HEIGHT - (nrects + 1) * ysep) / nrects;
        float xpos = (WIDTH - rectWidth) / 2;
        float ypos = ysep;
        for (int i = 0; i < nrects; i++)
        {
            sf::RectangleShape rect;
            rect.setSize({ rectWidth, rectHeight });
            rect.setFillColor(DEFAULT_COLOR);
            rect.setPosition(xpos, ypos);
            mCenterRects.push_back(std::move(rect));
            ypos += rectHeight + ysep;
        }
    }
    void render(sf::RenderTarget& rt)
    {
        for (auto& r : mCenterRects)
        {
            rt.draw(r);
        }
    }
private:
    std::vector<sf::RectangleShape> mCenterRects;
};
class Game
{
public:
    Game() : mWindow(sf::VideoMode(WIDTH, HEIGHT), "Pong")
    {
        mWindow.setVerticalSyncEnabled(true);
        mCpu.setPos(WIDTH - PADDLE_WITDH - 10, HEIGHT / 2);
        mBall.setPos(WIDTH / 2 - BALL_R - 10, 
            HEIGHT / 2);
        mPlayer.setPos(PADDLE_WITDH + 10, HEIGHT / 2);
    }
    void run()
    {
        sf::Clock clock;
        while (mWindow.isOpen())
        {
            float dt = clock.restart().asSeconds();
            events();
            update(dt);
            render();
        }
    }
private:
    void update(float dt)
    {
        mPlayer.update(dt);
        mCpu.updateCpu(dt, mBall.getPos().y);
        mBall.update(dt);
        mBall.collidesWith(mPlayer, HUMAN);
        mBall.collidesWith(mCpu, CPU);
    }
    void events()
    {
        sf::Event ev;
        while (mWindow.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) mWindow.close();
        }
    }
    void render()
    {
        mWindow.clear();
        mBg.render(mWindow);
        mBall.render(mWindow);
        mPlayer.render(mWindow);
        mCpu.render(mWindow);
        mWindow.display();
    }
    sf::RenderWindow mWindow;
    Background mBg;
    Ball mBall;
    Paddle mCpu;
    Paddle mPlayer;
};

int main()
{  
    srand(time(nullptr));
    Game game;
    game.run();
    return 0;
}
