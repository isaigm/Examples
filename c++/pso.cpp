#include <iostream>
#include <vector>
#include <cmath>

template <class T>
struct Point
{
	Point<T> operator - (Point<T> other)
	{
		return Point<T> { x - other.x, y - other.y };
	}
	Point<T> operator * (T scalar)
	{
		return Point<T> { scalar * x, scalar * y};
	}
	Point<T> operator + (Point<T> other)
	{
		return Point<T> { x + other.x, y + other.y };
	}
	T x{}, y{};
};

double evaluate(Point<double> point)
{
	return std::pow(point.x - 3.14, 2) + std::pow(point.y - 2.72, 2) + std::sin(3 * point.x + 1.41) + std::sin(4 * point.y - 1.73);
}
double getRandom(double min, double max)
{
	return double(rand()) / double(RAND_MAX) * (max - min) + min;
}
struct Particle
{
	double bestValue {INT_MAX};
	Point<double> position;
	Point<double> bestPosition;
	Point<double> velocity;
	void update(Point<double> globalBest, double c1, double c2, double w)
	{
		position = position + velocity;
		double currValue = evaluate(position);
		if (currValue < bestValue)
		{
			bestValue    = currValue;
			bestPosition = position;
		}
		double r1 = getRandom(0, 1);
		double r2 = getRandom(0, 1);

		velocity = velocity * w + (bestPosition - position) * c1 * r1 + (globalBest - position) * c2 * r2;

	}
};
struct PSO
{
	PSO(size_t size) : particles(size) {

	}
	void run(int iterations, double w, double c1, double c2)
	{
		Point<double> globalBest;
		double bestValue{ INT_MAX };
		auto getBest = [&]()
		{
			for (auto& particle : particles)
			{
				double currValue = evaluate(particle.position);
				if (currValue < bestValue)
				{
					bestValue  = currValue;
					globalBest = particle.position;
				}
			}
		};
		for (auto& particle : particles)
		{
			particle.position = { getRandom(0, 5), getRandom(0, 5) };
		}
		getBest();
		for (int i = 0; i < iterations; i++)
		{
			for (auto& particle : particles)
			{
				particle.update(globalBest, c1, c2, w);
			}
			getBest();
		}
		std::cout << globalBest.x << "," << globalBest.y << "\n";
		std::cout << bestValue << "\n";
	}
	std::vector<Particle> particles;
};
int main()
{
	srand(time(0));
	PSO pso(200);
	pso.run(1000, 0.7, 0.1, 0.1);
	return 0;
}
