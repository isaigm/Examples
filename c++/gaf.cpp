#include <vector>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
// https://arxiv.org/pdf/1506.00327.pdf
namespace GAF
{
    enum GAFType
    {
        GASF,
        GADF
    };
    float map(float n, float x1, float x2, float y1, float y2)
    {
        float m = (y2 - y1) / (x2 - x1);
        return y1 + m * (n - x1);
    }
    float lerp(float a, float b, float t)
    {
        return t * (b - a) + a;
    }
    void genColors(std::vector<sf::Color> &colors, sf::Color startColor, sf::Color endColor, int n)
    {
        float t = 0;
        float dt = 1 / float(n);
        for (int i = 0; i < n; i++)
        {
            int r = lerp(startColor.r, endColor.r, t);
            int g = lerp(startColor.g, endColor.g, t);
            int b = lerp(startColor.b, endColor.b, t);
            sf::Color color(r, g, b);
            colors.push_back(color);
            t += dt;
        }
    }
    void convertToImage(std::vector<float> &timeSeries, GAFType type)
    {
        float max = timeSeries[0];
        float min = timeSeries[0];
        for (auto x : timeSeries)
        {
            max = std::max(max, x);
            min = std::min(min, x);
        }
        auto scale = [&](float x)
        {
            return ((x - max) + (x - min)) / (max - min);
        };

        std::vector<float> scaled;

        for (auto x : timeSeries)
        {
            scaled.push_back(scale(x));
        }
        size_t N = timeSeries.size();
        std::vector<std::vector<float>> image;
        for (size_t i = 0; i < scaled.size(); i++)
        {
            scaled[i] = std::acos(scaled[i]);
            std::vector<float> row(N);
            image.push_back(std::move(row));
        }
        sf::Image resultImg;
        std::vector<sf::Color> gradient;
        genColors(gradient, sf::Color(0x7019FFFF), sf::Color(0x52F5CBFF), 128);
        genColors(gradient, sf::Color(0x52F5CBFF), sf::Color(0xAFF498FF), 128);
        genColors(gradient, sf::Color(0xAFF498FF), sf::Color(0xF4C069FF), 128);
        genColors(gradient, sf::Color(0xF4C069FF), sf::Color::Red, 128);

        resultImg.create(N, N);
        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < N; j++)
            {
                if (type == GASF)
                {
                    image[i][j] = std::cos(scaled[i] + scaled[j]);
                }
                else if (type == GADF)
                {
                    image[i][j] = std::sin(scaled[i] - scaled[j]);
                }
                int idx = map(image[i][j], -1, 1, 0, gradient.size() - 1);
                resultImg.setPixel(i, j, gradient[idx]);
            }
        }
        resultImg.saveToFile("output.png");
    }
};
int main()
{
    std::vector<float> timeSeries;

    float t = 0;
    float dt = 10.0f / 1024.0f;
    for (int i = 0; i < 1024; i++)
    {
        timeSeries.push_back(std::sin(2 * M_PI * t));
        t += dt;
    }
    GAF::convertToImage(timeSeries, GAF::GASF);
    return 0;
}
