#include <iostream>
#include <SFML/Graphics.hpp>
#include <omp.h>
#include <vector>
#include <math.h>
#include<windows.h>
#define WIDTH 800
#define HEIGHT 600

bool isPrime(int k)
{
	for (int i = 2; i * i <= k; i++)
	{
		if (k % i == 0) return false;
	}
	return true;
}
void makeSpirals(sf::RenderTexture& rt, int n, float zoom)
{
	sf::VertexArray vertexArray(sf::PrimitiveType::Points);
	float x = WIDTH / 2;
	float y = HEIGHT / 2;
	rt.clear();
	#pragma omp parallel for 
	for (int i = 2; i <= n; i++)
	{
		if (isPrime(i))
		{
			sf::Vertex point;
			point.position.x = x + (i * cos(i)) / zoom;
			point.position.y = y - (i * sin(i)) / zoom;
			point.color = sf::Color(0xa8d5e5);
			#pragma omp critical
			{
				vertexArray.append(point);
			}
		}
	}
	rt.draw(vertexArray);
	rt.display();
}

int main()
{
	omp_set_num_threads(4);
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "3B1B");
	sf::Sprite sp;
	sf::RenderTexture rt;
	window.setVerticalSyncEnabled(true);
	rt.create(WIDTH, HEIGHT);
	sp.setTexture(rt.getTexture());
	float zoom = 1;
	int points = 1;
	bool end = false;
	float totalTime = 0;
	sf::Clock clock;
	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
		}
		if (zoom <= 1000)
		{
			makeSpirals(rt, points, zoom);
			points += 10000;
			zoom += 10;
			totalTime += dt;
		}
		else if (!end)
		{
			sp.getTexture()->copyToImage().saveToFile("primes.png");
			end = true;
			std::cout << "Total time: " << totalTime << "\n";
		}
		window.clear();
		window.draw(sp);
		window.display();
	}
	return 0; 
}
