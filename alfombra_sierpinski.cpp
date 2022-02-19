#include <SFML/Graphics.hpp>
#include <iostream>
sf::RectangleShape crear_cuadro(float x, float y, float lado)
{
    sf::RectangleShape cuadro;

    cuadro.setSize({lado, lado});
    cuadro.setPosition(x - lado / 2, y - lado / 2);
    cuadro.setFillColor(sf::Color::Black);
    return cuadro;
}
void sierp(sf::RenderTexture &rt, float lado_real, float lado, float x, float y, int n)
{
    if (n == 0)
        return;
    rt.draw(crear_cuadro(x * 600, y * 600, lado_real));

    sierp(rt, lado_real / 3.0f, lado / 3.0f, x / 3.0f, y / 3.0f, n - 1);
    sierp(rt, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 1.0f / 3.0f, y / 3.0f, n - 1);
    sierp(rt, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 2.0f / 3.0f, y / 3.0f, n - 1);
    sierp(rt, lado_real / 3.0f, lado / 3.0f, x / 3.0f, y / 3.0f + 1 / 3.0f, n - 1);
    sierp(rt, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 2 / 3.0f, y / 3.0f + 1 / 3.0f, n - 1);
    sierp(rt, lado_real / 3.0f, lado, x / 3.0f, y / 3.0f + 2 / 3.0f, n - 1);
    sierp(rt, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 1 / 3.0f, y / 3.0f + 2 / 3.0f, n - 1);
    sierp(rt, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 2 / 3.0f, y / 3.0f + 2 / 3.0f, n - 1);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Alfombra de sierpinski");
    window.setVerticalSyncEnabled(true);
    sf::Sprite sprite;
    sf::RenderTexture rt;
    rt.create(600, 600);
    
    rt.clear(sf::Color::White);
    sierp(rt, 600 / 3, 1, 0.5f, 0.5f, 5);
    rt.display();
    sprite.setTexture(rt.getTexture());
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

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
