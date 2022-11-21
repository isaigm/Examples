#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>
#include <set>
#define LEN_CITIES 50
#define CITY_RADIUS 3
#define POPULATION 200
#define WIDTH 1280
#define HEIGHT 720

class city
{
public:
	city(float x, float y) : pos(x, y)
	{
	}
	void render(sf::RenderTarget& rt)
	{
		sf::CircleShape circle(CITY_RADIUS);
		circle.setFillColor(sf::Color::White);
		circle.setOrigin({ CITY_RADIUS, CITY_RADIUS });
		circle.setPosition(pos);
		rt.draw(circle);
	}
	sf::Vector2f getPosition()
	{
		return pos;
	}
private:
	sf::Vector2f pos;
};
int get_rnd(int min, int max)
{
	return rand() % (max - min) + min;
}
std::random_device rd;
std::mt19937 g(rd());
std::vector<city> cities;
class agent {
public:
	agent(agent& p1, agent& p2)
	{
		std::set<int> visited;
		auto &c1 = p1.getChromosomes();
		auto &c2 = p2.getChromosomes();
		for (int i = 0; i < LEN_CITIES; i++)
		{
			if (visited.find(c1[i]) == visited.end())
			{
				chromosomes.push_back(c1[i]);
				visited.insert(c1[i]);
			}
			if (visited.find(c2[i]) == visited.end())
			{
				chromosomes.push_back(c2[i]);
				visited.insert(c2[i]);
			}
		}
		updateFitness();
	}
	agent()
	{
		for (int i = 0; i < LEN_CITIES; i++)
		{
			chromosomes.push_back(i);
		}
		std::shuffle(chromosomes.begin(), chromosomes.end(), g);
		updateFitness();
	}
	void mutate()
	{
		int firstIdx = rand() % LEN_CITIES;
		int secondIdx = rand() % LEN_CITIES;
		while (secondIdx == firstIdx)
		{
			secondIdx = rand() % LEN_CITIES;
		}
		std::swap(chromosomes[firstIdx], chromosomes[secondIdx]);
		updateFitness();
	}
	float getFitness()
	{
		return fitness;
	}
	std::vector<int> &getChromosomes()
	{
		return chromosomes;
	}
private:
	void updateFitness()
	{
		fitness = 0;
		for (int i = 0; i < LEN_CITIES - 1; i++)
		{
			auto originCity = cities[chromosomes[i]];
			auto destCity = cities[chromosomes[i + 1]];
			float dx = originCity.getPosition().x - destCity.getPosition().x;
			float dy = originCity.getPosition().y - destCity.getPosition().y;
			fitness += sqrt(dx * dx + dy * dy);
		}
	}
	float fitness = 0;
	std::vector<int> chromosomes;
};
void draw_path(agent& a, sf::RenderTarget& rt)
{
	auto& path = a.getChromosomes();
	for (int i = 0; i < LEN_CITIES - 1; i++)
	{
		auto originCity = cities[path[i]];
		auto destCity = cities[path[i + 1]];
		sf::Vertex line[2];
		line[0].position = originCity.getPosition();
		line[0].color = sf::Color::Red;
		line[1].position = destCity.getPosition();
		line[1].color = sf::Color::Red;
		rt.draw(line, 2, sf::Lines);
	}
}
int main()
{
	srand(time(nullptr));
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "TSP genetic");
	std::vector<agent> population;
	window.setVerticalSyncEnabled(true);
	sf::Clock clock;
	for (int i = 0; i < LEN_CITIES; i++)
	{
		float x = get_rnd(CITY_RADIUS, WIDTH - CITY_RADIUS);
		float y = get_rnd(CITY_RADIUS, HEIGHT - CITY_RADIUS);
		cities.push_back(city(x, y));
	}
	for (int i = 0; i < POPULATION; i++)
	{
		population.push_back(agent());
	}
	sf::Font font;
	font.loadFromFile("C:\\Users\\isaig\\Downloads\\Arial.ttf");
	sf::Text hud;
	hud.setFont(font);
	hud.setPosition(10, 10);
	hud.setCharacterSize(16);
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
		std::sort(population.begin(), population.end(), [](agent& a1, agent& a2) {
			return a1.getFitness() < a2.getFitness();
			});
		
		int parents = get_rnd(10, 50);
		for (int i = 0; i < parents; i++)
		{
			population.pop_back();
		}
		std::vector<agent> newAgents;
		for (int i = 0, j = 0; i < parents; i++, j += 2)
		{
			agent a;
			if (rand() & 1)
			{
				a = agent(population[j], population[j + 1]);
			}else a = agent(population[j + 1], population[j]);
			if (rand() % 6 < 3) a.mutate();
			newAgents.push_back(a);
		}
		for (int i = 0; i < parents; i++)
		{
			population.push_back(newAgents[i]);
		}
		window.clear();
		draw_path(population[0], window);
		char msg[64];
		snprintf(msg, sizeof(msg), "distancia total actual %.4f", population[0].getFitness());
		hud.setString(msg);
		for (auto& c : cities)
		{
			c.render(window);
		}
		window.draw(hud);
		window.display();
	}
	return 0;
}
