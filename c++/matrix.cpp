#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#define WIDTH 800
#define HEIGHT 600
#define CHAR_SIZE 16

static sf::Font font;
class column
{
public:
    column(float start_x, float start_y)
    {
        head_pos.x = start_x;
        head_pos.y = start_y;
        if (printable_ascii.size() == 0)
        {
            for (int i = 32; i < 127; i++)
            {
                printable_ascii.push_back(i);
            }
        }
    }
    void update(float dt)
    {
        for (auto &letter : col)
        {
            auto color = letter.getFillColor();
            if (color.a - dt * speed <= 0)
            {
                letter.setString(random_letter());
                color.a = 0xFF;
            }
            else
            {
                color.a -= dt * speed;
            }

            letter.setFillColor(color);
        }
        if (head_pos.y + CHAR_SIZE <= HEIGHT)
        {
            sf::Text text;
            text.setFont(font);
            text.setFillColor(sf::Color::Green);
            text.setPosition(head_pos);
            text.setLineSpacing(0.6f);
            text.setString(random_letter());
            text.setLetterSpacing(0.6f);
            text.setCharacterSize(CHAR_SIZE);
            head_pos.y += text.getCharacterSize();
            col.push_back(text);
        }
    }
    void draw(sf::RenderTarget &rt)
    {
        for (auto &letter : col)
        {
            rt.draw(letter);
        }
    }

private:
    std::string random_letter()
    {
        std::string letter;
        letter.push_back(printable_ascii[rand() % printable_ascii.size()]);
        return letter;
    }
    std::vector<sf::Text> col;
    sf::Vector2f head_pos;
    std::string printable_ascii;
    float speed = rand() % 150 + 50; // que tan rapido se desvanecen las letras
};
void add_column(std::vector<column> &cols, float &start_x)
{
    auto col = column(start_x, 0);
    cols.push_back(std::move(col));
    start_x += CHAR_SIZE;
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "matrix");
    window.setFramerateLimit(30);
    if (!font.loadFromFile("DroidSansMono.ttf"))
        exit(1);
    srand(time(nullptr));
    std::vector<column> cols;
    float start_x = 0;
    sf::Clock clock;
    float total_time = 0;
    add_column(cols, start_x);
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();
        }
        total_time += dt;
        if (total_time >= 0.1f)
        {
            if (cols.size() < 50)
            {
                add_column(cols, start_x);
            }
            total_time = 0;
        }
        for (auto &col : cols)
        {
            col.update(dt);
        }
        window.clear(sf::Color::Black);
        for (auto &col : cols)
        {
            col.draw(window);
        }
        window.display();
    }
    return 0;
}
