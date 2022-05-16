#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#define WIDTH 512
#define HEIGHT 512


float lerp(float a, float b, float t)
{

    return a  + (b-a) * t;
}
void genTexture(sf::RenderTexture& rt)
{
    float values[256][256];
    float* noiseMap = new float[WIDTH * HEIGHT];
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            float val = rand() / float(RAND_MAX);
            values[i][j] = val;
        }
    }
    float maxn = 0;
    
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
           
            float sum = 0;
            float freq = 0.02f;
            for (int k = 0; k < 7; k++)
            {
                float xx = j * std::pow(2, k) * freq;
                float yy = i * std::pow(2, k) * freq;
                
                int jj = floor(xx);
                int ii = floor(yy);
                float tx = xx - floor(xx);
                float ty = yy - floor(yy);
                ii = ii % 256;
                jj = jj % 256;
                float upleft = values[ii][jj];
                float downleft = values[(ii + 1) % 256][jj];
                float upright = values[ii][(jj + 1) % 256];
                float downright = values[(ii + 1) % 256][(jj + 1) % 256];
                tx = (1 - cos(tx * 3.14159f)) * 0.5f;
                ty = (1 - cos(ty * 3.14159f)) * 0.5f;
                float a = lerp(upleft, upright, tx);
                float b = lerp(downleft, downright, tx);
                float c = lerp(a, b, ty);
                
                sum += c / std::pow(2, k);
            }
            noiseMap[j + i * WIDTH] = sum;
            maxn = std::max(maxn, sum);
           
        }
    }
    rt.clear();
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            float val = noiseMap[j + i * WIDTH] / maxn;
            sf::Color color(val*255, val * 255/2, 0);
            

            sf::Vertex point;
            point.color = color;
            point.position = { float(j), float(i) };
            rt.draw(&point, 1, sf::Points);
        }
    }
    rt.display();
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Value noise");
    window.setVerticalSyncEnabled(true);
    srand(time(nullptr));
    sf::RenderTexture rt;
    sf::Sprite sp;
    rt.create(WIDTH, HEIGHT);
    genTexture(rt);
    sp.setTexture(rt.getTexture());
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
        window.clear(sf::Color::Black);
        window.draw(sp);
        window.display();
      
    }
    return 0;
}
