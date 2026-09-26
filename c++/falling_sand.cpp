#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include <cstdint>
#include <utility>
#include <algorithm>

const int WIDTH = 1280;
const int HEIGHT = 720;
const int MAX_PARTICLES = 30;
const float MAX_RADIUS = 20.0f;
const float PARTICLES_RATIO = 0.2f;
const float PI = 3.14159f;
const int SCALE = 4;
const int GRID_W = WIDTH / SCALE;
const int GRID_H = HEIGHT / SCALE;
const int BRUSH_RADIUS = 3;
const sf::Color SAND_COLOR = sf::Color(230, 204, 138);

enum class CellType
{
    EMPTY,
    SAND,
    WATER,
    WALL
};

const char *materialName(CellType type)
{
    switch (type)
    {
    case CellType::SAND:  return "Falling sand - Water";
    case CellType::WATER: return "Falling sand - Sand";
    case CellType::WALL:  return "Falling sand - Wall";
    default:              return "Falling sand";
    }
}

std::random_device rd;
std::mt19937 mt(rd());
float random(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(mt);
}

struct grid_t
{
    grid_t()
        : m_texture(sf::Vector2u(GRID_W, GRID_H)),
          m_sprite(m_texture),
          m_cells(GRID_W * GRID_H, CellType::EMPTY),
          m_pixels(GRID_W * GRID_H * 4),
          m_moved(GRID_W * GRID_H, false)
    {
        render();
        m_sprite.setScale({SCALE, SCALE});
        m_sprite.setPosition({0, 0});
    }

    void putAt(sf::Vector2f pos, CellType type)
    {
        int nParticles = random(MAX_PARTICLES * PARTICLES_RATIO, MAX_PARTICLES);
        for (int k = 0; k < nParticles; k++)
        {
            float r = MAX_RADIUS * std::sqrt(random(0, 1));
            float angle = random(0, 2 * PI);
            float x = pos.x + r * std::cos(angle);
            float y = pos.y + r * std::sin(angle);
            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
                continue;
            int i = y / SCALE;
            int j = x / SCALE;
            if (isEmpty(j, i))
                setCellAt(type, j, i);
        }
    }

    void fillCircle(sf::Vector2f pos, CellType type, int radius)
    {
        int cx = static_cast<int>(std::floor(pos.x / SCALE));
        int cy = static_cast<int>(std::floor(pos.y / SCALE));
        for (int dy = -radius; dy <= radius; dy++)
        {
            for (int dx = -radius; dx <= radius; dx++)
            {
                if (dx * dx + dy * dy > radius * radius)
                    continue;
                int x = cx + dx;
                int y = cy + dy;
                if (x < 0 || x >= GRID_W || y < 0 || y >= GRID_H)
                    continue;
                setCellAt(type, x, y);
            }
        }
    }

   
    void fillLine(sf::Vector2f a, sf::Vector2f b, CellType type, int radius)
    {
        sf::Vector2f d = b - a;
        float len = std::sqrt(d.x * d.x + d.y * d.y);
        float step = radius * SCALE * 0.5f; 
        int n = std::max(1, static_cast<int>(std::ceil(len / step)));
        for (int k = 0; k <= n; k++)
            fillCircle(a + d * (static_cast<float>(k) / n), type, radius);
    }

    CellType getCellAt(int x, int y)
    {
        return m_cells[x + y * GRID_W];
    }

    void setCellAt(CellType type, int x, int y)
    {
        m_cells[x + y * GRID_W] = type;
    }

    bool isEmpty(int x, int y)
    {
        if (x < 0 || x >= GRID_W || y < 0 || y >= GRID_H)
            return false;
        return getCellAt(x, y) == CellType::EMPTY;
    }

    bool isEmptyOrWater(int x, int y)
    {
        if (x < 0 || x >= GRID_W || y < 0 || y >= GRID_H)
            return false;
        auto t = getCellAt(x, y);
        return t == CellType::EMPTY || (t == CellType::WATER && !m_moved[x + y * GRID_W]);
    }

    void move(int x1, int y1, int x2, int y2)
    {
        std::swap(m_cells[x1 + y1 * GRID_W], m_cells[x2 + y2 * GRID_W]);
        m_moved[x1 + y1 * GRID_W] = true;
        m_moved[x2 + y2 * GRID_W] = true;
    }

    void update()
    {
        std::fill(m_moved.begin(), m_moved.end(), false);
        for (int i = GRID_H - 1; i >= 0; i--)
        {
            for (int j = 0; j < GRID_W; j++)
            {
                auto type = getCellAt(j, i);
                if (type == CellType::SAND)
                {
                    int dir = random(0, 1) > 0.5f ? 1 : -1;
                    if (isEmptyOrWater(j, i + 1))
                        move(j, i, j, i + 1);
                    else if (isEmptyOrWater(j + dir, i + 1))
                        move(j, i, j + dir, i + 1);
                    else if (isEmptyOrWater(j - dir, i + 1))
                        move(j, i, j - dir, i + 1);
                }
                else if (type == CellType::WATER && !m_moved[j + i * GRID_W])
                {
                    int dir = random(0, 1) > 0.5f ? 1 : -1;
                    if (isEmpty(j, i + 1))
                        move(j, i, j, i + 1);
                    else if (isEmpty(j + dir, i + 1))
                        move(j, i, j + dir, i + 1);
                    else if (isEmpty(j - dir, i + 1))
                        move(j, i, j - dir, i + 1);
                    else if (isEmpty(j + dir, i))
                        move(j, i, j + dir, i);
                    else if (isEmpty(j - dir, i))
                        move(j, i, j - dir, i);
                }
            
            }
        }
        render();
    }

    void render()
    {
        for (int i = 0; i < GRID_H; i++)
        {
            for (int j = 0; j < GRID_W; j++)
            {
                switch (getCellAt(j, i))
                {
                case CellType::EMPTY:
                    setColorAt(sf::Color::White, j, i);
                    break;
                case CellType::SAND:
                    setColorAt(SAND_COLOR, j, i);
                    break;
                case CellType::WATER:
                    setColorAt(sf::Color::Blue, j, i);
                    break;
                case CellType::WALL:
                    setColorAt(sf::Color(100, 100, 100), j, i);
                    break;
                }
            }
        }
        m_texture.update(m_pixels.data());
    }

    void setColorAt(sf::Color color, int x, int y)
    {
        int index = x + y * GRID_W;
        m_pixels[index * 4] = color.r;
        m_pixels[index * 4 + 1] = color.g;
        m_pixels[index * 4 + 2] = color.b;
        m_pixels[index * 4 + 3] = color.a;
    }

    sf::Texture m_texture;
    sf::Sprite m_sprite;
    std::vector<CellType> m_cells;
    std::vector<std::uint8_t> m_pixels;
    std::vector<bool> m_moved;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Falling sand");
    grid_t grid;

    window.setVerticalSyncEnabled(true);

    CellType current = CellType::SAND;
    window.setTitle(materialName(current));

    sf::Vector2f lastMouse;
    bool wasDrawing = false;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

      
        CellType previous = current;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
            current = CellType::SAND;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
            current = CellType::WATER;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
            current = CellType::WALL;
        if (current != previous)
            window.setTitle(materialName(current));

        auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        bool left = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        bool right = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

      
        sf::Vector2f from = wasDrawing ? lastMouse : mousePos;

        if (left)
        {
            if (current == CellType::WALL)
                grid.fillLine(from, mousePos, CellType::WALL, BRUSH_RADIUS);
            else
                grid.putAt(mousePos, current);
        }
        if (right)
            grid.fillLine(from, mousePos, CellType::EMPTY, BRUSH_RADIUS); 

        wasDrawing = left || right;
        lastMouse = mousePos;

        grid.update();
        window.clear();
        window.draw(grid.m_sprite);
        window.display();
    }
    return 0;
}
