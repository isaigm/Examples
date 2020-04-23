#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#define WIDTH 800
#define HEIGHT 600
using namespace std;
using Point = sf::Vector2f;
using Pixel = sf::Vector2i;
class App
{
public:
    App()
    {
        window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Prueba");
        window->setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - WIDTH) / 2, (sf::VideoMode::getDesktopMode().height - HEIGHT) / 2));
        window->setFramerateLimit(60);
        float t = 0;
        float r = 1;
        while (t < 2 * M_PI)
        {
            Point p;
            p.x = r * sin(t);
            p.y = r * cos(t);
            points.push_back(p);
            t += 0.1f;
        }
        cout << points.size() << endl;
    }
    ~App()
    {
        delete window;
    }
    void run()
    {
        while (window->isOpen())
        {
            events();
            draw();
        }
    }

private:
    void draw()
    {
        window->clear(sf::Color::White);
        for (size_t i = 0; i < points.size(); i++)
        {
            sf::CircleShape pixel(2);
            pixel.setPosition(static_cast<Point>(getPixelAt(points[i])));
            pixel.setFillColor(sf::Color::Black);
            window->draw(pixel);
        }
        window->display();
    }
    void events()
    {
        sf::Event e;
        while (window->pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
                window->close();
                break;
            default:
                break;
            }
        }
    }
    sf::RenderWindow *window;
    Pixel getPixelAt(Point &p)
    {
        Pixel pixel;
        pixel.x = floor(abs(p.x - xui) / dpx);
        pixel.y = floor(abs(p.y - yui) / dpy);
        return pixel;
    }
    float xui = -3; //x [-3, 3]
    float xuf = 3;
    float yui = -3; //y [-3, 3]
    float yuf = 3;
    float dpx = abs(xuf - xui) / WIDTH;  //pixeles en x por unidad cartesiana
    float dpy = abs(yuf - yui) / HEIGHT; //pixeles en y por unidad cartesiana
    std::vector<Point> points;
};

int main()
{
    App app;
    app.run();
    return 0;
}
