#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#define BOX_H 50
#define BOW_W 50
sf::RectangleShape create_box(float x, float y)
{
	sf::RectangleShape box({ BOW_W, BOX_H });
	box.setFillColor(sf::Color::White);
	box.setOutlineColor(sf::Color::Black);
	box.setOutlineThickness(1.2f);
	box.setPosition({ x, y });
	return box;
}
int main()
{
	std::map<sf::Keyboard::Key, bool> pressed_keys;
	std::map<sf::Keyboard::Key, sf::RectangleShape> keys;
	sf::RenderWindow window(sf::VideoMode(500, 400), "Simple key monitor");
	keys[sf::Keyboard::Left]	= std::move(create_box(250, 250));
	keys[sf::Keyboard::Down]	= std::move(create_box(310, 250));
	keys[sf::Keyboard::Right]	= std::move(create_box(370, 250));
	keys[sf::Keyboard::Up]		= std::move(create_box(310, 190));
	keys[sf::Keyboard::Z]		= std::move(create_box(50, 250));
	keys[sf::Keyboard::X]		= std::move(create_box(110, 250));
	keys[sf::Keyboard::C]		= std::move(create_box(170, 250));
	keys[sf::Keyboard::A]		= std::move(create_box(50, 190));
	keys[sf::Keyboard::S]		= std::move(create_box(110, 190));
	keys[sf::Keyboard::D]		= std::move(create_box(170, 190));
	keys[sf::Keyboard::W]		= std::move(create_box(110, 120));
	keys[sf::Keyboard::E]		= std::move(create_box(170, 120));
	window.setFramerateLimit(80);
	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		pressed_keys[sf::Keyboard::Left]	= sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		pressed_keys[sf::Keyboard::Down]	= sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		pressed_keys[sf::Keyboard::Right]	= sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		pressed_keys[sf::Keyboard::Up]		= sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		pressed_keys[sf::Keyboard::Z]		= sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
		pressed_keys[sf::Keyboard::X]		= sf::Keyboard::isKeyPressed(sf::Keyboard::X);
		pressed_keys[sf::Keyboard::C]		= sf::Keyboard::isKeyPressed(sf::Keyboard::C);
		pressed_keys[sf::Keyboard::A]		= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		pressed_keys[sf::Keyboard::S]		= sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		pressed_keys[sf::Keyboard::D]		= sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		pressed_keys[sf::Keyboard::W]		= sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		pressed_keys[sf::Keyboard::E]		= sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		for (const auto& p : pressed_keys)
		{
			if (p.second)
			{
				keys[p.first].setFillColor(sf::Color::Red);
			}
			else
			{
				keys[p.first].setFillColor(sf::Color::White);
			}
		}
		window.clear(sf::Color::White);
		for (const auto& k : keys)
		{
			window.draw(k.second);
		}
		window.display();
	}
	return 0;
}

