#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#define WIDTH 800
#define HEIGHT 600
#define NTAPS 20
//por hacer: circular buffer
float map(float n, float x1, float x2, float y1, float y2)
{
    float m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}
class Filter
{

public:
    Filter()
    {
        for (int i = 0; i <= NTAPS; i++)
        {
            coef[i] = 1.0f / (NTAPS + 1); // porque el filtro es moving average
            delayLine.push_back(0);
        }
    }
    void draw(sf::RenderTarget &rt)
    {
        rt.draw(inputSignal.data(), inputSignal.size(), sf::LineStrip);
        rt.draw(outputSignal.data(), outputSignal.size(), sf::LineStrip);

    }
    void addPoint(float x, float y)
    {
        float realx = map(x, -13, 13, 0, WIDTH);
        float realy = map(y, -10, 10, HEIGHT, 0);
        float output = fir(y) - 4;
        sf::Vertex ipoint;
        sf::Vertex opoint;
        ipoint.position = {realx, realy};
        ipoint.color = sf::Color::Red;
        opoint.color = sf::Color::Green;
        opoint.position = {realx, map(output, -10, 10, HEIGHT, 0)};
        if (realx >= WIDTH && !fmaxSize)
        {
            fmaxSize = true;
        }
        else if (fmaxSize)
        {
            for (int i = 0; i < inputSignal.size() - 1; i++)
            {
                inputSignal[i].position.y = inputSignal[i + 1].position.y;
                outputSignal[i].position.y = outputSignal[i + 1].position.y;
            }
            ipoint.position.x = inputSignal.back().position.x;
            inputSignal[inputSignal.size() - 1].position = ipoint.position;

            opoint.position.x = outputSignal.back().position.x;
            outputSignal[inputSignal.size() - 1].position = opoint.position;
        }
        else
        {
            inputSignal.push_back(ipoint);
            outputSignal.push_back(opoint);
        }
    }

private:
    float fir(float y)
    {
        float output = 0;
        for (int i = NTAPS; i >= 1; i--)
        {
            delayLine[i] = delayLine[i - 1];
        }
        delayLine[0] = y;
        for (int i = 0; i <= NTAPS; i++)
        {
            output += delayLine[i] * coef[i];
        }
        return output;
    }
    std::vector<float> delayLine;
    std::vector<sf::Vertex> inputSignal;
    std::vector<sf::Vertex> outputSignal;
    float coef[NTAPS + 1];
    bool fmaxSize = false;
};
void drawAxis(sf::RenderTarget &rt)
{
    sf::Vertex xaxis[2];
    xaxis[0].position = {0, HEIGHT / 2};
    xaxis[0].color = sf::Color::Blue;
    xaxis[1].position = {WIDTH, HEIGHT / 2};
    xaxis[1].color = xaxis[0].color;
    sf::Vertex yaxis[2];
    yaxis[0].position = {WIDTH / 2, 0};
    yaxis[0].color = sf::Color::Blue;
    yaxis[1].position = {WIDTH / 2, HEIGHT};
    yaxis[1].color = yaxis[0].color;
    rt.draw(xaxis, 2, sf::Lines);
    rt.draw(yaxis, 2, sf::Lines);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "FIR");
    window.setVerticalSyncEnabled(true);
    srand(time(nullptr));
    Filter fir;
    float alpha = -4 * M_PI;
    while (window.isOpen())
    {
        float amp = (rand() / float(RAND_MAX));
        if (rand() % 2)
            amp *= -1;
        float x = alpha;
        float y = sin(alpha) + amp + 4;

        fir.addPoint(x, y);
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        window.clear();
        drawAxis(window);
        fir.draw(window);
        window.display();
        alpha += 0.08f;
    }
    return 0;
}
