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
static sf::Vector2i WORLD_SIZE = { 65, 65 };
float fade(float t) {
    return ((6 * t - 15) * t + 10) * t * t * t;
}

float lerp(float t, float a1, float a2) {
    return a1 + t * (a2 - a1);
}
float operator *(sf::Vector2f v1, sf::Vector2f v2)
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
        int X = int(std::floor(x)) % 256;
        int Y = int(std::floor(y)) % 256;
        
       
        float xf = x - std::floor(x);
        float yf = y - std::floor(y);
        auto topRight = sf::Vector2f(xf - 1.0f, yf - 1.0f);
        auto topLeft =  sf::Vector2f(xf, yf - 1.0f);
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

        return lerp(u,lerp(v, dotBottomLeft, dotTopLeft),
          lerp(v, dotBottomRight, dotTopRight)
        );
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
        sprite.setTextureRect({j * TW, i * TH, TW, TH});

        sprite.scale(0.2, 0.2);
        lastY = 0;
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
    float lastY;
};
sf::Vector2i invert(float x, float y, float h, float w, float a, float b)
{
    float ri = (2 / h) * (y - b) - (x - a) * (1 / w);
    float rj = (4 / h) * (y - b) - ri;
    return { int(rj), int(ri) };
}
void genWorld(std::vector<tile>& world, sf::RenderTexture& rt)
{
    perlin_noise noise;
    float min = 100000;
    float max = 0;
    //rt.clear();
    for (int i = 0; i < WORLD_SIZE.y; i++)
    {
        for (int j = 0; j < WORLD_SIZE.x; j++)
        {
            float sum = 0;

            float a = 1.0f;
            float f = 0.06f;
            for (int k = 0; k < 6; k++)
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
            min = std::min(min, sum);
            max = std::max(max, sum);

            auto &t = world[j + i * WORLD_SIZE.x];
           
            float rx = (j - i) * float(20) / 2.0f + 700;
            float ry = (j + i) * float(20) / 4.0f + 10;
            t.setPos(rx, ry);
           
            sf::Color color;

            if(sum < 0.2) color = sf::Color(220, 220, 220);
            else if (sum > 0.2 && sum < 0.5) color = sf::Color::Green;
            else color = sf::Color::Blue;
            t.setColor(color);
            //t.render(rt);
         
        }
        
    }
    //rt.display();
    std::cout << min << "\n";
    std::cout << max << "\n";
}
int main()
{
   
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Perlin noise");
    window.setVerticalSyncEnabled(true);
    sf::RenderTexture rt;
    sf::Sprite sp;
    rt.create(2000, 2000);
    std::vector<tile> world;
    
    //genTexture(rt);
   
    sf::Texture tiles;
    if (!tiles.loadFromFile("C:\\Users\\isaig\\Downloads\\block_atlas.png"))
    {
        return 1;
    }
    for (int i = 0; i < WORLD_SIZE.x * WORLD_SIZE.y; i++)
    {
        world.emplace_back(tiles, 0, 1);
    }
    genWorld(world, rt);
    sp.setTexture(rt.getTexture());
    sf::Clock clock;
    float time = 0;
    sf::View view(sf::FloatRect(0, 0, WIDTH, HEIGHT));
    
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
            else if (ev.type == sf::Event::MouseWheelMoved)
            {
                view.zoom(1 - ev.mouseWheel.delta * 0.1);
                window.setView(view);
            }
            else if (ev.type == sf::Event::MouseButtonPressed)
            {
                auto pos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pos);

                auto t = invert(worldPos.x, worldPos.y, 20, 20, 700, 10);

                std::cout << worldPos.x << "," << worldPos.y << "\n";
                if (t.x >= 0 && t.x < WORLD_SIZE.x && t.y >= 0 && t.y < WORLD_SIZE.y)
                {
                   
                    world[t.x + t.y * WORLD_SIZE.y].setColor(sf::Color::Red);
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            view.move(0, -dt * 200);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            view.move(0, dt * 200);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            view.move(-dt * 200, 0);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            view.move(dt * 200, 0);
        }
      
       
        window.clear();
        window.setView(view);
        //window.draw(sp);
        for (auto& t : world)
        {
            t.render(window);
        }
        window.display();
       
      
    }
    return 0;
}

