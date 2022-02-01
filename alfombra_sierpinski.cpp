#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
sf::RectangleShape crear_cuadro(float x, float y, float lado)
{
    sf::RectangleShape cuadro;

    cuadro.setSize({lado, lado});
    cuadro.setPosition(x - lado / 2, y - lado/2);
    cuadro.setFillColor(sf::Color::Black);
    return cuadro;
}
void sierp(std::vector<sf::RectangleShape> &cuadros, float lado_real, float lado, float x, float y, int n)
{
    if (n == 0)
        return;
    cuadros.push_back(crear_cuadro(x * 699, y * 699, lado_real));

    sierp(cuadros, lado_real / 3.0f, lado / 3.0f, x / 3.0f, y / 3.0f, n - 1);
    sierp(cuadros, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 1.0f / 3.0f, y / 3.0f, n - 1);
    sierp(cuadros, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 2.0f / 3.0f, y / 3.0f, n - 1);
    sierp(cuadros, lado_real / 3.0f, lado / 3.0f, x / 3.0f, y / 3.0f + 1 / 3.0f, n - 1);
    sierp(cuadros, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 2 / 3.0f, y / 3.0f + 1 / 3.0f, n - 1);
    sierp(cuadros, lado_real / 3.0f, lado, x / 3.0f, y / 3.0f + 2 / 3.0f, n - 1);
    sierp(cuadros, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 1 / 3.0f, y / 3.0f + 2 / 3.0f, n - 1);
    sierp(cuadros, lado_real / 3.0f, lado / 3.0f, x / 3.0f + 2 / 3.0f, y / 3.0f + 2 / 3.0f, n - 1);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(699, 699), "Alfombra de sierpinski");
    std::vector<sf::RectangleShape> cuadros;
    window.setVerticalSyncEnabled(true);

    sierp(cuadros, 699/3, 1, 0.5f, 0.5f,5);
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
        for (auto &cuadro : cuadros)
        {
            window.draw(cuadro);
        }
        window.display();
    }

    return 0;
}
