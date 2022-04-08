#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#define M_PI  3.14159265358979323846f 
#define WIDTH 800
#define HEIGHT 600

float map(float n, float x1, float x2, float y1, float y2)
{
    float m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}
class Curve
{

public:
    void draw(sf::RenderTarget &rt)
    {
        rt.draw(pixels.data(), pixels.size(), sf::LineStrip);
    }
    void fromPoints(std::vector<sf::Vector2f> &points)
    {
        for (auto& p : points)
        {
            addPoint(p.x, p.y);
        }
    }
    void addPoint(float x, float y)
    {
        float realx = map(x, -13, 13, 0, WIDTH);
        float realy = map(y, -10, 10, HEIGHT, 0);
        sf::Vertex pixel;
        pixel.position = { realx, realy };
        pixel.color = sf::Color::Red;
        pixels.push_back(pixel);
    }
    void clear()
    {
        pixels.clear();
    }
private:
    std::vector<sf::Vertex> pixels;
};
void drawAxis(sf::RenderTarget &rt) {
    sf::Vertex xaxis[2];
    xaxis[0].position = { 0, HEIGHT / 2 };
    xaxis[0].color = sf::Color::Blue;
    xaxis[1].position = { WIDTH, HEIGHT / 2 };
    xaxis[1].color = xaxis[0].color;
    sf::Vertex yaxis[2];
    yaxis[0].position = { WIDTH / 2, 0 };
    yaxis[0].color = sf::Color::Blue;
    yaxis[1].position = { WIDTH / 2, HEIGHT };
    yaxis[1].color = yaxis[0].color;
    rt.draw(xaxis, 2, sf::Lines);
    rt.draw(yaxis, 2, sf::Lines);
}
void movingAverage(int n, std::vector<sf::Vector2f>& inputSignal, std::vector<sf::Vector2f> &outputSignal)
{

    for (int i = 0; i < inputSignal.size() - n; i++)
    {
        float x = inputSignal[i].x;
        float yOut = 0;

        for (int j = i; j < i + n; j++)
        {
            yOut += inputSignal[j].y;
        }
        outputSignal.push_back({ x, yOut / n });
    }
    for (int i = inputSignal.size() - n; i < inputSignal.size(); i++)
    {
        float x = inputSignal[i].x;
        float yOut = 0;

        for (int j = i; j < inputSignal.size(); j++)
        {
            yOut += inputSignal[j].y;
        }
        outputSignal.push_back({ x, yOut / (inputSignal.size() - i)});
    }
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Moving average");
    window.setVerticalSyncEnabled(true);
    srand(time(nullptr));
    Curve noisySine;
    std::vector<sf::Vector2f> inputSignal;
    std::vector < sf::Vector2f> outputSignal;
    for (float alpha = -4 * M_PI; alpha < 4 * M_PI; alpha += 0.01f)
    {
        float amp = (rand() / float(RAND_MAX));
        if (rand() % 2) amp *= -1;
        float x = alpha;
        float y = sin(alpha) + amp + 4;
        inputSignal.push_back({ x, y });
        noisySine.addPoint(x, y);
    }
    int n = 10;
    movingAverage(n, inputSignal, outputSignal);
    for (auto& p : outputSignal)
    {
        p.y -= 4;
    }
    Curve filteredSignal;
    filteredSignal.fromPoints(outputSignal);
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
            if (ev.type == sf::Event::MouseWheelMoved)
            {
                if (ev.mouseWheel.delta > 0) {
                    n++;
                }
                else n--;
                if (n < 0)
                {
                    n = 1;
                }
                outputSignal.clear();
                movingAverage(n, inputSignal, outputSignal);
                filteredSignal.clear();
                for (auto& p : outputSignal)
                {
                    p.y -= 4;
                }
                filteredSignal.fromPoints(outputSignal);

            }
        }
        window.clear();
        drawAxis(window);
        noisySine.draw(window);
        filteredSignal.draw(window);
        window.display();
    }
    return 0;
}
