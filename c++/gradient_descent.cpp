#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <iostream>
#define WIDTH 800
#define HEIGHT 600
const int N_POINTS = 100;
std::random_device rd;
std::mt19937 gen{rd()};
std::uniform_real_distribution<float> dist(0.0f, 1.0f);

float map(float n, float x1, float x2, float y1, float y2)
{
    float m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}
float real_function(float x)
{
    return x * x - 0.5f;
}

sf::Vector3f partial(std::vector<float> &x, std::vector<float> &y, float a, float b, float c)
{
    float partial_dev = 0.0f;
    sf::Vector3f result{};
    for (int i = 0; i < x.size(); i++)
    {
        float xi = x[i];
        float yi = y[i];
        float temp = 2 * (yi - a * xi * xi - b * xi - c);
        result.x += temp * (-xi * xi);
        result.y += temp * -xi;
        result.z += temp * -1;
    }
    result /= float(x.size());
    return result;
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "");
    window.setVerticalSyncEnabled(true);

    std::vector<sf::Vertex> points;
    std::vector<float> x;
    std::vector<float> y;
    float curr_x = -1;
    for (int i = 0; i < N_POINTS; i++)
    {

        sf::Vertex point;
        point.color = sf::Color::Red;
        curr_x += 2 / float(N_POINTS);
        float sign = dist(gen) < 0.5f ? 1 : -1;
        float curr_y = real_function(curr_x) + sign * dist(gen) / 50.0f;
        x.push_back(curr_x);
        y.push_back(curr_y);
        sf::Vector2f realPos = {map(curr_x, -1, 1, 0, WIDTH), map(curr_y, -1, 1, HEIGHT, 0)};
        point.position = realPos;
        points.push_back(point);
    }
    sf::Vector3f coef = {dist(gen), dist(gen), dist(gen)};
    float lr = 0.05f;

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
        std::vector<sf::Vertex> predicted_points;
        for (int i = 0; i < N_POINTS; i++)
        {
            sf::Vertex point;
            float xx = x[i];
            float yy = coef.x * (xx * xx) + coef.y * xx + coef.z;
            point.color = sf::Color::Green;
            sf::Vector2f realPos = {map(xx, -1, 1, 0, WIDTH), map(yy, -1, 1, HEIGHT, 0)};
            point.position = realPos;
            predicted_points.push_back(point);
        }
        coef -= lr * partial(x, y, coef.x, coef.y, coef.z);
        window.clear();
        window.draw(points.data(), points.size(), sf::Points);
        window.draw(predicted_points.data(), predicted_points.size(), sf::Points);
        window.display();
    }
    return 0;
}
