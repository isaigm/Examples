#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdio>
#include <stack>
#include <cstdlib>
#define ROWS 20
#define COLS 20
#define W 25
#define H 25
sf::RectangleShape get_rect(int w, int h)
{
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Black);
    rect.setSize({w, h});
    return rect;
}
std::vector<std::pair<int, int>> get_unvisited_neighbours(std::vector<sf::RectangleShape> &rects, uint8_t visited[ROWS][COLS], int x, int y)
{
    std::vector<std::pair<int, int>> neighbours;
    auto valid_coords = [](int x, int y) -> bool {
        return !(x < 0 || x >= COLS || y < 0 || y >= ROWS);
    };
    int nw = W - 5;
    int nh = H - 5;
    int x1 = (x * W + 150) - nw / 2;
    int y1 = (y * H + 50) - nh / 2;
    if (valid_coords(x - 1, y))
    {
        if (visited[y][x - 1] != 1)
        {
            auto r = get_rect(nw, nh);
            r.setPosition({x1 - nw, y1});
            rects.push_back(std::move(r));
            neighbours.push_back(std::make_pair(x - 1, y));
        }
    }
    if (valid_coords(x + 1, y))
    {
        if (visited[y][x + 1] != 1)
        {
            auto r = get_rect(nw, nh);
            r.setPosition({x1 + nw, y1});
            rects.push_back(std::move(r));
            neighbours.push_back(std::make_pair(x + 1, y));
        }
    }
    if (valid_coords(x, y + 1))
    {
        if (visited[y + 1][x] != 1)
        {
            auto r = get_rect(nw, nh);
            r.setPosition({x1, y1 + nh});
            rects.push_back(std::move(r));
            neighbours.push_back(std::make_pair(x, y + 1));
        }
    }
    if (valid_coords(x, y - 1))
    {
        if (visited[y - 1][x] != 1)
        {
            auto r = get_rect(nw, nh);
            r.setPosition({x1, y1 - nh});
            rects.push_back(std::move(r));
            neighbours.push_back(std::make_pair(x, y - 1));
        }
    }
    return neighbours;
}
void dfs(std::vector<sf::RectangleShape> &rects, uint8_t visited[ROWS][COLS], int x, int y)
{
    std::stack<std::pair<int, int>> path_stack;
    path_stack.push(std::make_pair(x, y));
    int nw = W - 5;
    int nh = H - 5;
    while (!path_stack.empty())
    {
        auto [x, y] = path_stack.top();
        int x1 = x * H + 150;
        int y1 = y * W + 50;
        auto rect = get_rect(nw, nh);
        rect.setPosition(x1 - nw / 2, y1 - nh / 2);
        rects.push_back(std::move(rect));
        path_stack.pop();
        auto neighbours = get_unvisited_neighbours(rects, visited, x, y);
        if (!neighbours.empty())
        {
            size_t idx = rand() % neighbours.size();
            for (size_t i = 0; i < neighbours.size(); i++)
            {
                auto [x1, y1] = neighbours[i];
                visited[y1][x1] = 1;
                if (idx != i)
                    path_stack.push(neighbours[i]);
            }
            path_stack.push(neighbours[idx]);
        }
    }
}
int main()
{
    uint8_t visited[ROWS][COLS] = {0};
    srand(time(nullptr));
    std::vector<sf::RectangleShape> rects;
    dfs(rects, visited, 0, 0);
    sf::RenderWindow window(sf::VideoMode(800, 600), "Maze generator");
    sf::Event ev;
    while (window.isOpen())
    {
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();
            else if (ev.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    for (int i = 0; i < ROWS; i++)
                    {
                        for (int j = 0; j < COLS; j++)
                        {
                            visited[i][j] = 0;
                        }
                    }
                    rects.clear();
                    dfs(rects, visited, 0, 0);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    sf::Texture texture;
                    texture.create(window.getSize().x, window.getSize().y);
                    texture.update(window);
                    if (texture.copyToImage().saveToFile("maze.png"))
                    {
                        printf("save screenshot succesfully\n");
                    }
                }
            }
        }
        window.clear(sf::Color::White);
        for (const auto &r : rects)
        {
            window.draw(r);
        }
        window.display();
    }
    return 0;
}
