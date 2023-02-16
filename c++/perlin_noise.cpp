#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
static const int WIDTH = 1920;
static const int HEIGHT = 1080;
static const int w = 256;
static const int TW = 100;
static const int TH = 100;
static sf::Vector2i WORLD_SIZE = { 99, 99 };
static const float M_PI = 3.14159f;
static const float CAM_SPEED = 250.0f;
float fade(float t)
{
    return ((6 * t - 15) * t + 10) * t * t * t;
}
float lerp(float t, float a1, float a2)
{
    return a1 + t * (a2 - a1);
}
float operator*(sf::Vector2f v1, sf::Vector2f v2)
{
    float x = v1.x * v2.x;
    float y = v1.y * v2.y;
    return x + y;
}
sf::Vector2f getVector(int v)
{
    int idx = v % 4;
    if (idx == 0)
        return sf::Vector2f(1.0, 1.0);
    else if (idx == 1)
        return sf::Vector2f(-1.0, 1.0);
    else if (idx == 2)
        return sf::Vector2f(-1.0, -1.0);
    else
        return sf::Vector2f(1.0, -1.0);
}
class perlin_noise
{
public:
    perlin_noise() : g(rd())
    {
        for (int i = 0; i < w; i++)
        {
            perm.push_back(i);
        }
        std::shuffle(perm.begin(), perm.end(), g);
        for (int i = 0; i < w; i++)
        {
            perm.push_back(perm[i]);
        }
    }
    float getVal(float x, float y)
    {
        int X = int(std::abs(std::floor(x))) % 256;
        int Y = int(std::abs(std::floor(y))) % 256;
        float xf = x - std::floor(x);
        float yf = y - std::floor(y);
        auto topRight = sf::Vector2f(xf - 1.0f, yf - 1.0f);
        auto topLeft = sf::Vector2f(xf, yf - 1.0f);
        auto bottomRight = sf::Vector2f(xf - 1.0f, yf);
        auto bottomLeft = sf::Vector2f(xf, yf);
        int size = 2 * w;
        int valueTopRight = perm[perm[X + 1] + Y + 1];
        int valueTopLeft = perm[perm[X] + Y + 1];
        int valueBottomRight = perm[perm[X + 1] + Y];
        int valueBottomLeft = perm[perm[X] + Y];

        float dotTopRight = topRight * getVector(valueTopRight);
        float dotTopLeft = topLeft * getVector(valueTopLeft);
        float dotBottomRight = bottomRight * getVector(valueBottomRight);
        float dotBottomLeft = bottomLeft * getVector(valueBottomLeft);

        float u = fade(xf);
        float v = fade(yf);

        return lerp(u, lerp(v, dotBottomLeft, dotTopLeft),
            lerp(v, dotBottomRight, dotTopRight));
    }

private:
    std::random_device rd;
    std::mt19937 g;
    std::vector<int> perm;
};

class tile
{
public:
    tile(sf::Texture& text, int i, int j)
    {
        sprite.setTexture(text);
        sprite.setTextureRect({ j * TW, i * TH, TW, TH });
        sprite.scale(0.2, 0.2);
    }
    void setGridPos(int i, int j)
    {
        gridPos.x = j;
        gridPos.y = i;
    }
    sf::Vector2i getGridPos()
    {
        return gridPos;
    }
    void render(sf::RenderTarget& rt)
    {
        rt.draw(sprite);
    }
    void setPos(float x, float y)
    {
        sprite.setPosition(x, y);
    }
    void setTile(int i, int j)
    {
        sprite.setTextureRect({ j * TW, i * TH, TW, TH });
    }
    void setColor(sf::Color c)
    {
        sprite.setColor(c);
    }

private:
    sf::Sprite sprite;
    sf::Vector2i gridPos;
};
sf::Vector2i invert(float x, float y, float h, float w, float a, float b)
{
    float ri = (2 / h) * (y - b) - (x - a) * (1 / w);
    float rj = (4 / h) * (y - b) - ri;
    return { int(rj), int(ri) };
}
class chunk
{
public:
    std::vector<tile>& getWorld()
    {
        return world;
    }
    chunk()
    {
        if (!tiles.loadFromFile("C:\\Users\\isaig\\Downloads\\block_atlas.png"))
        {
            exit(1);
        }
        for (int i = 0; i < WORLD_SIZE.y; i++)
        {
            for (int j = 0; j < WORLD_SIZE.x; j++)

            {
                addTile(i, j);
            }
        }
        genWorld(WORLD_SIZE.y * WORLD_SIZE.x);
    }
    sf::Vector2f getCenter()
    {
        auto& t = world[world.size() / 2];
        int i = t.getGridPos().y;
        int j = t.getGridPos().x;
        float rx = (j - i) * float(20) / 2.0f;
        float ry = (j + i) * float(20) / 4.0f;
        return { rx, ry };
    }
    void render(sf::RenderTarget& rt)
    {
        for (auto& t : world)
        {
            t.render(rt);
        }
    }
    void moveRight()
    {
        auto it = std::remove_if(world.begin(), world.end(), [=](tile& t)
            {
                int i = t.getGridPos().y;
        int j = t.getGridPos().x;
        int lastRow = firstRow + WORLD_SIZE.y - 1;
        return i == lastRow || j == firstCol; });
        world.erase(it, world.end());
        firstCol++;
        firstRow--;
        int size = 0;
        for (int i = firstCol; i < firstCol + WORLD_SIZE.x; i++)
        {
            addTile(firstRow, i);
        }
        for (int i = firstRow + 1; i < firstRow + WORLD_SIZE.y; i++)
        {
            addTile(i, firstCol + WORLD_SIZE.x - 1);
        }
        genWorld(WORLD_SIZE.x + WORLD_SIZE.y - 1);
        sortTiles();
    }
    void moveLeft()
    {
        auto it = std::remove_if(world.begin(), world.end(), [=](tile& t)
            {
                int i = t.getGridPos().y;
        int j = t.getGridPos().x;
        int lastCol = firstCol + WORLD_SIZE.x - 1;
        return i == firstRow || j == lastCol; });
        world.erase(it, world.end());
        firstCol--;
        firstRow++;
        for (int i = firstRow; i < firstRow + WORLD_SIZE.y; i++)
        {
            addTile(i, firstCol);
        }
        for (int i = firstCol + 1; i < firstCol + WORLD_SIZE.x; i++)
        {
            addTile(firstRow + WORLD_SIZE.y - 1, i);
        }
        genWorld(WORLD_SIZE.x + WORLD_SIZE.y - 1);
        sortTiles();
    }
    void moveUp()
    {
        auto it = std::remove_if(world.begin(), world.end(), [=](tile& t)
            {
                int i = t.getGridPos().y;
        int j = t.getGridPos().x;
        int lastCol = firstCol + WORLD_SIZE.x - 1;
        int lastRow = firstRow + WORLD_SIZE.y - 1;
        return i == lastRow || j == lastCol; });
        world.erase(it, world.end());
        firstCol--;
        firstRow--;
        for (int i = firstRow + 1; i < firstRow + WORLD_SIZE.y; i++)
        {
            addTile(i, firstCol);
        }
        for (int i = firstCol; i < firstCol + WORLD_SIZE.x; i++)
        {
            addTile(firstRow, i);
        }
        genWorld(WORLD_SIZE.x + WORLD_SIZE.y - 1);
        sortTiles();
    }
    void moveDown()
    {
        auto it = std::remove_if(world.begin(), world.end(), [=](tile& t)
            {
                int i = t.getGridPos().y;
        int j = t.getGridPos().x;
        return i == firstRow || j == firstCol; });
        world.erase(it, world.end());
        firstRow++;
        for (int i = firstRow; i < firstRow + WORLD_SIZE.y - 1; i++)
        {
            addTile(i, firstCol + WORLD_SIZE.x);
        }
        firstCol++;
        for (int i = firstCol; i < firstCol + WORLD_SIZE.x; i++)
        {
            addTile(firstRow + WORLD_SIZE.y - 1, i);
        }
        genWorld(WORLD_SIZE.x + WORLD_SIZE.y - 1);
        sortTiles();
    }

private:
    void addTile(int i, int j)
    {
        world.emplace_back(tiles, 0, 0);
        world.back().setGridPos(i, j);
    }
    void sortTiles()
    {
        std::sort(world.begin(), world.end(), [](tile& t1, tile& t2)
            {
                auto pos1 = t1.getGridPos();
        auto pos2 = t2.getGridPos();
        if (pos1.y == pos2.y) return pos1.x < pos2.x;
        return pos1.y < pos2.y; });
    }
    int firstRow = 0;
    int firstCol = 0;
    std::vector<tile> world;
    perlin_noise noise;
    sf::Texture tiles;
    void genWorld(int size)
    {

        for (int k = world.size() - 1; k >= 0; k--)
        {

            auto& t = world[k];
            int i = t.getGridPos().y;
            int j = t.getGridPos().x;
            float sum = 0;

            float a = 1.0f;
            float f = 0.06f;
            for (int k = 0; k < 2; k++)
            {
                float xx = j * f;
                float yy = i * f;
                float n = a * noise.getVal(xx, yy);

                sum += n;
                a *= 0.5f;
                f *= 2.0f;
            }
            sum += 1.0f;
            sum *= 0.5f;
           
            float rx = 0;
            float ry = 0;
            int h = int(sum * 6);
            rx = (j - i) * float(20) / 2.0f;
            ry = (j + i) * float(20) / 4.0f + h * 10;

            t.setPos(rx, ry);

            sf::Color color;

            if (sum < 0.2)
                color = sf::Color(220, 220, 220);
            else if (sum > 0.2 && sum < 0.5)
                color = sf::Color::Green;
            else
                color = sf::Color::Blue;

            t.setColor(color);


            size--;
            if (size <= 0) break;
        }
       
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Perlin noise");
    window.setVerticalSyncEnabled(true);
    sf::Clock clock;
    float time = 0;
    sf::View view(sf::FloatRect(0, 0, WIDTH, HEIGHT));
    chunk c;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        char fps[64];
        snprintf(fps, sizeof(fps), "FPS: %.3f", 1.0f / dt);
        window.setTitle(fps);
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
            else if (ev.type == sf::Event::MouseWheelMoved)
            {
                view.zoom(1 - ev.mouseWheel.delta * 0.1);
                window.setView(view);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            c.moveDown();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            c.moveUp();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            c.moveLeft();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            c.moveRight();
        }
        view.setCenter(c.getCenter());

        window.clear();
        window.setView(view);
        c.render(window);
        window.display();
    }
    return 0;
}
