#include <SFML/Graphics.hpp>
#include <vector>
#include <string.h>
#include <iostream>
#define WIDTH 200
#define HEIGHT 200
std::vector<std::pair<int, int>> coords1;
std::vector<std::pair<int, int>> coords2;
int get_alive_neighbours(std::vector<std::pair<int, int>> &coords, uint8_t *cells, int row, int col)
{
    int alive = 0;
    for (auto &p : coords)
    {
        int i = (p.first + row) % HEIGHT;
        int j = (p.second + col) % WIDTH;

        if (cells[j + i * WIDTH])
            alive++;
    }
    return alive;
}
void step(uint8_t *cells, uint8_t *next_cells)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            float avg1 = get_alive_neighbours(coords1, cells, i, j) / float(coords1.size());
            float avg2 = get_alive_neighbours(coords2, cells, i, j) / float(coords2.size());
            int state = cells[j + i * WIDTH];
            /*
            if (avg1 >= 0.210f && avg1 <= 0.220f)
            {
                state = 1;
            }
            if (avg1 >= 0.350f && avg1 <= 0.500f)
            {
                state = 0;
            }
            if (avg1 >= 0.750f && avg1 <= 0.850f)
            {
                state = 0;
            }
            if (avg2 >= 0.100f && avg2 <= 0.280f)
            {
                state = 0;
            }
            if (avg2 >= 0.430f && avg2 <= 0.550f)
            {
                state = 1;
            }
            if (avg1 >= 0.120f && avg1 <= 0.150f)
            {
                state = 0;
            }
            */
            if (avg1 >= 0.185 && avg1 <= 0.200)
            {
                state = 1;
            }
            if (avg1 >= 0.343 && avg1 <= 0.580)
            {
                state = 0;
            }
            if (avg1 >= 0.750 && avg1 <= 0.850)
            {
                state = 0;
            }
            if (avg2 >= 0.150 && avg2 <= 0.280)
            {
                state = 0;
            }
            if (avg2 >= 0.445 && avg2 <= 0.680)
            {
                state = 1;
            }
            if (avg1 >= 0.150 && avg1 <= 0.180)
            {
                state = 0;
            }
            next_cells[j + i * WIDTH] = state;
        }
    }
    memcpy(cells, next_cells, sizeof(uint8_t) * WIDTH * HEIGHT);
}
void fill_random(uint8_t *cells, uint8_t *next_cells)
{
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        int state = rand() % 2;
        cells[i] = state;
        next_cells[i] = state;
    }
}
void convert_to_pixels(uint8_t *cells, sf::Uint8 *pixels)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (cells[j + i * WIDTH])
            {
                pixels[(j + i * WIDTH) * 4] = 255;
                pixels[(j + i * WIDTH) * 4 + 1] = 255;
                pixels[(j + i * WIDTH) * 4 + 2] = 255;
                pixels[(j + i * WIDTH) * 4 + 3] = 255;
            }
            else
            {
                pixels[(j + i * WIDTH) * 4] = 0;
                pixels[(j + i * WIDTH) * 4 + 1] = 0;
                pixels[(j + i * WIDTH) * 4 + 2] = 0;
                pixels[(j + i * WIDTH) * 4 + 3] = 255;
            }
        }
    }
}
int main()
{
    srand(time(nullptr));

    coords1.push_back({0, -7});
    coords1.push_back({0, -6});
    coords1.push_back({0, -5});
    coords1.push_back({-1, -7});
    coords1.push_back({-1, -6});
    coords1.push_back({-1, -5});
    coords1.push_back({-2, -7});
    coords1.push_back({-2, -6});
    coords1.push_back({-2, -5});
    coords1.push_back({1, -5});
    coords1.push_back({1, -6});
    coords1.push_back({1, -7});
    coords1.push_back({2, -7});
    coords1.push_back({2, -6});
    coords1.push_back({2, -5});
    coords1.push_back({3, -6});
    coords1.push_back({3, -4});
    coords1.push_back({3, -5});
    coords1.push_back({4, -6});
    coords1.push_back({4, -5});
    coords1.push_back({4, -4});
    coords1.push_back({4, -3});
    coords1.push_back({5, -5});
    coords1.push_back({5, -4});
    coords1.push_back({5, -3});
    coords1.push_back({5, -2});
    coords1.push_back({5, -1});
    coords1.push_back({5, 0});
    coords1.push_back({5, 1});
    coords1.push_back({5, 2});
    coords1.push_back({5, 3});
    coords1.push_back({5, 4});
    coords1.push_back({5, 5});
    coords1.push_back({6, -4});
    coords1.push_back({6, -3});
    coords1.push_back({6, -2});
    coords1.push_back({6, -1});
    coords1.push_back({6, 0});
    coords1.push_back({6, 1});
    coords1.push_back({6, 2});
    coords1.push_back({6, 3});
    coords1.push_back({6, 4});
    coords1.push_back({7, -2});
    coords1.push_back({7, -1});
    coords1.push_back({7, 0});
    coords1.push_back({7, 1});
    coords1.push_back({7, 2});
    coords1.push_back({4, 3});
    coords1.push_back({4, 4});
    coords1.push_back({4, 5});
    coords1.push_back({4, 6});
    coords1.push_back({3, 6});
    coords1.push_back({3, 5});
    coords1.push_back({3, 4});
    coords1.push_back({2, 5});
    coords1.push_back({2, 6});
    coords1.push_back({2, 7});
    coords1.push_back({1, 5});
    coords1.push_back({1, 6});
    coords1.push_back({1, 7});
    coords1.push_back({0, 7});
    coords1.push_back({0, 6});
    coords1.push_back({0, 5});
    coords1.push_back({-1, 5});
    coords1.push_back({-2, 5});
    coords1.push_back({-2, 6});
    coords1.push_back({-1, 6});
    coords1.push_back({-2, 7});
    coords1.push_back({-1, 7});
    coords1.push_back({-3, -6});
    coords1.push_back({-3, -5});
    coords1.push_back({-3, -4});
    coords1.push_back({-4, -6});
    coords1.push_back({-4, -5});
    coords1.push_back({-4, -4});
    coords1.push_back({-4, -3});
    coords1.push_back({-5, -5});
    coords1.push_back({-5, -4});
    coords1.push_back({-5, -3});
    coords1.push_back({-5, -2});
    coords1.push_back({-5, -1});
    coords1.push_back({-5, 0});
    coords1.push_back({-5, 1});
    coords1.push_back({-5, 2});
    coords1.push_back({-5, 3});
    coords1.push_back({-5, 4});
    coords1.push_back({-5, 5});
    coords1.push_back({-4, 5});
    coords1.push_back({-3, 5});
    coords1.push_back({-4, 6});
    coords1.push_back({-3, 6});
    coords1.push_back({-3, 4});
    coords1.push_back({-4, 4});
    coords1.push_back({-4, 3});
    coords1.push_back({-6, -4});
    coords1.push_back({-6, -3});
    coords1.push_back({-6, -2});
    coords1.push_back({-6, -1});
    coords1.push_back({-6, 0});
    coords1.push_back({-6, 1});
    coords1.push_back({-6, 2});
    coords1.push_back({-6, 3});
    coords1.push_back({-6, 4});
    coords1.push_back({-7, -2});
    coords1.push_back({-7, -1});
    coords1.push_back({-7, 0});
    coords1.push_back({-7, 1});
    coords1.push_back({-7, 2});

    coords2.push_back({0, -1});
    coords2.push_back({-1, -1});
    coords2.push_back({-1, 0});
    coords2.push_back({-1, 1});
    coords2.push_back({0, 1});
    coords2.push_back({1, 1});
    coords2.push_back({1, 0});
    coords2.push_back({1, -1});
    coords2.push_back({-2, -2});
    coords2.push_back({-1, -2});
    coords2.push_back({0, -2});
    coords2.push_back({1, -2});
    coords2.push_back({2, -2});
    coords2.push_back({2, -1});
    coords2.push_back({2, 0});
    coords2.push_back({2, 1});
    coords2.push_back({2, 2});
    coords2.push_back({1, 2});
    coords2.push_back({0, 2});
    coords2.push_back({-1, 2});
    coords2.push_back({-2, 2});
    coords2.push_back({-2, 1});
    coords2.push_back({-2, 0});
    coords2.push_back({-2, -1});
    coords2.push_back({3, -1});
    coords2.push_back({3, 0});
    coords2.push_back({3, 1});
    coords2.push_back({1, 3});
    coords2.push_back({0, 3});
    coords2.push_back({-1, 3});
    coords2.push_back({-1, -3});
    coords2.push_back({0, -3});
    coords2.push_back({1, -3});
    coords2.push_back({-3, -1});
    coords2.push_back({-3, 0});
    coords2.push_back({-3, 1});

    sf::RenderWindow window(sf::VideoMode(WIDTH * 2, HEIGHT * 2), "MNCA");
    sf::Texture texture;
    texture.create(WIDTH, HEIGHT);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.scale(2, 2);
    uint8_t *cells = new uint8_t[WIDTH * HEIGHT];
    uint8_t *next_cells = new uint8_t[WIDTH * HEIGHT];
    sf::Uint8 *pixels = new sf::Uint8[WIDTH * HEIGHT * 4];
    fill_random(cells, next_cells);
    while (window.isOpen())

    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        step(cells, next_cells);
        convert_to_pixels(cells, pixels);
        texture.update(pixels);
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }
    return 0;
}
