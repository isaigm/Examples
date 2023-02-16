#include <iostream>
#include <SFML/Graphics.hpp>
#define ROWS 120
#define COLS 160

void draw_state(uint8_t cells[ROWS][COLS], sf::RenderTarget &window, sf::Uint8 *pixels, sf::Texture &texture, sf::Sprite &sprite)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			switch (cells[i][j])
			{
			case 0:
				pixels[(j + i * COLS) * 4] = 0;
				pixels[(j + i * COLS) * 4 + 1] = 0;
				pixels[(j + i * COLS) * 4 + 2] = 0;
				pixels[(j + i * COLS) * 4 + 3] = 0;
				break;
			case 1:
				pixels[(j + i * COLS) * 4] = 255;
				pixels[(j + i * COLS) * 4 + 1] = 0;
				pixels[(j + i * COLS) * 4 + 2] = 0;
				pixels[(j + i * COLS) * 4 + 3] = 255;
				break;
			case 2:
				pixels[(j + i * COLS) * 4] = 255;
				pixels[(j + i * COLS) * 4 + 1] = 255;
				pixels[(j + i * COLS) * 4 + 2] = 255;
				pixels[(j + i * COLS) * 4 + 3] = 255;
				break;
			default:
				break;
			}
		}
	}
	texture.update(pixels);
	window.draw(sprite);
}
void step(uint8_t cells[ROWS][COLS], uint8_t copy[ROWS][COLS])
{
	auto get_alive_neighbours = [cells](int i, int j) {
		int total = 0;
		for (int i1 = i - 1; i1 <= i + 1; i1++)
		{
			for (int j1 = j - 1; j1 <= j + 1; j1++)
			{
				if (i1 == i && j1 == j)
				{
					continue;
				}
				int i2 = i1 % ROWS;
				int j2 = j1 % COLS;
				if (cells[i2][j2] == 2)
					total++;
			}
		}
		return total;
	};
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			switch (cells[i][j])
			{
			case 0:
				if (get_alive_neighbours(i, j) == 2)
				{
					copy[i][j] = 2;
				}
				break;
			case 1:
				copy[i][j] = 0;
				break;
			case 2:
				copy[i][j] = 1;
				break;
			default:
				break;
			}
		}
	}
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			cells[i][j] = copy[i][j];
		}
	}
}
int main()
{
	srand(time(nullptr));
	uint8_t cells[ROWS][COLS];
	uint8_t copy[ROWS][COLS];
	sf::RenderWindow window(sf::VideoMode(800, 600), "Brian's Brain");
	sf::Event ev;
	sf::Uint8 *pixels = new sf::Uint8[ROWS * COLS * 4];
	sf::Texture texture;
	sf::Sprite sprite;
	texture.create(COLS, ROWS);
	sprite.setScale(5, 5);
	sprite.setTexture(texture);
	window.setVerticalSyncEnabled(true);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			cells[i][j] = rand() % 3;
			copy[i][j] = cells[i][j];
		}
	}
	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		step(cells, copy);
		window.clear();
		draw_state(cells, window, pixels, texture, sprite);
		window.display();
	}
	return 0;
}


