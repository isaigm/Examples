#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <limits>
#include <SFML/Graphics.hpp>

constexpr int MAP_WIDTH = 20;
constexpr int MAP_HEIGHT = 20;
constexpr int CELL_SIZE = 40;
constexpr int NUM_ANTS = 50;
constexpr float ALPHA = 1.0f;
constexpr float BETA = 2.5f;
constexpr float EVAPORATION = 0.05f;
constexpr float Q = 50.0f;
constexpr float INITIAL_PHEROMONE = 0.1f;

struct Point
{
    int x, y;
    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point &other) const { return !(*this == other); }
};

inline int getNodeIndex(int x, int y)
{
    return y * MAP_WIDTH + x;
}

inline float getDistance(Point a, Point b)
{
    float dx = static_cast<float>(a.x - b.x);
    float dy = static_cast<float>(a.y - b.y);
    return std::sqrt(dx * dx + dy * dy);
}

class AntColonySimulation
{
private:
    std::vector<bool> obstacles;
    std::vector<float> pheromones;

    Point startNode{0, 0};
    Point endNode{MAP_WIDTH - 1, MAP_HEIGHT - 1};

    std::vector<Point> bestPath;
    float bestPathLength = std::numeric_limits<float>::max();

    std::mt19937 rng;

    sf::VertexArray gridVertices;
    sf::VertexArray pathVertices;

public:
    AntColonySimulation() : obstacles(MAP_WIDTH * MAP_HEIGHT, true), rng(std::random_device{}())
    {
        int totalNodes = MAP_WIDTH * MAP_HEIGHT;
        pheromones.resize(totalNodes * totalNodes, INITIAL_PHEROMONE);

        // Inicializar obstáculos aleatorios
        std::uniform_int_distribution<int> distW(0, MAP_WIDTH - 1);
        std::uniform_int_distribution<int> distH(0, MAP_HEIGHT - 1);

        for (int i = 0; i < (MAP_WIDTH * MAP_HEIGHT) / 4; ++i)
        {
            int x = distW(rng);
            int y = distH(rng);
            if ((x == startNode.x && y == startNode.y) || (x == endNode.x && y == endNode.y))
                continue;
            obstacles[getNodeIndex(x, y)] = false;
        }

        initGraphics();
    }

    void setTarget(int x, int y)
    {
        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
        {
            endNode = {x, y};
            obstacles[getNodeIndex(x, y)] = true;
            resetPheromones();
        }
    }

    void resetPheromones()
    {
        std::fill(pheromones.begin(), pheromones.end(), INITIAL_PHEROMONE);
        bestPath.clear();
        bestPathLength = std::numeric_limits<float>::max();
    }

    void resetObstacles()
    {
        std::fill(obstacles.begin(), obstacles.end(), true);
        std::uniform_int_distribution<int> distW(0, MAP_WIDTH - 1);
        std::uniform_int_distribution<int> distH(0, MAP_HEIGHT - 1);
        for (int i = 0; i < (MAP_WIDTH * MAP_HEIGHT) / 4; ++i)
        {
            int x = distW(rng);
            int y = distH(rng);
            if ((x == startNode.x && y == startNode.y) || (x == endNode.x && y == endNode.y))
                continue;
            obstacles[getNodeIndex(x, y)] = false;
        }
        resetPheromones();
        initGraphics();
    }

    void update()
    {
        struct Ant
        {
            std::vector<Point> path;
            std::vector<bool> visited;
            float totalDist = 0;
            bool reached = false;

            Ant() : visited(MAP_WIDTH * MAP_HEIGHT, false) {}
        };

        std::vector<Ant> ants(NUM_ANTS);

        for (auto &ant : ants)
        {
            Point current = startNode;
            ant.path.reserve(MAP_WIDTH * MAP_HEIGHT);
            ant.path.push_back(current);
            ant.visited[getNodeIndex(current.x, current.y)] = true;

            int steps = 0;
            while (current != endNode && steps < MAP_WIDTH * MAP_HEIGHT)
            {
                std::vector<Point> neighbors;
                std::vector<float> probabilities;
                float probSum = 0;

                for (int dy = -1; dy <= 1; ++dy)
                {
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        if (dx == 0 && dy == 0)
                            continue;

                        Point next = {current.x + dx, current.y + dy};

                        if (next.x < 0 || next.x >= MAP_WIDTH || next.y < 0 || next.y >= MAP_HEIGHT)
                            continue;
                        int nextIdx = getNodeIndex(next.x, next.y);
                        if (!obstacles[nextIdx] || ant.visited[nextIdx])
                            continue;

                        int currIdx = getNodeIndex(current.x, current.y);
                        int edgeIdx = currIdx * (MAP_WIDTH * MAP_HEIGHT) + nextIdx;

                        float pheromone = std::pow(pheromones[edgeIdx], ALPHA);

                        float distToGoal = getDistance(next, endNode);
                        float stepDist = getDistance(current, next);
                        float heuristic = std::pow(1.0f / (distToGoal + 0.1f) * (1.0f / stepDist), BETA);

                        float prob = pheromone * heuristic;

                        neighbors.push_back(next);
                        probabilities.push_back(prob);
                        probSum += prob;
                    }
                }

                if (neighbors.empty())
                    break;

                std::uniform_real_distribution<float> dist(0.0f, probSum);
                float r = dist(rng);
                float cumulative = 0;
                int selectedIdx = -1;

                for (size_t i = 0; i < probabilities.size(); ++i)
                {
                    cumulative += probabilities[i];
                    if (r <= cumulative)
                    {
                        selectedIdx = i;
                        break;
                    }
                }
                if (selectedIdx == -1)
                    selectedIdx = neighbors.size() - 1;

                current = neighbors[selectedIdx];
                ant.path.push_back(current);
                ant.visited[getNodeIndex(current.x, current.y)] = true;
                ant.totalDist += getDistance(ant.path[ant.path.size() - 2], current);

                if (current == endNode)
                {
                    ant.reached = true;
                }
                steps++;
            }
        }

        for (auto &p : pheromones)
        {
            p *= (1.0f - EVAPORATION);
            if (p < INITIAL_PHEROMONE)
                p = INITIAL_PHEROMONE;
        }

        for (const auto &ant : ants)
        {
            if (!ant.reached)
                continue;

            // Actualizar mejor camino global
            if (ant.totalDist < bestPathLength)
            {
                bestPathLength = ant.totalDist;
                bestPath = ant.path;
                std::cout << "Nuevo mejor camino: " << bestPathLength << std::endl;
            }

            float deposit = Q / ant.totalDist;
            for (size_t i = 0; i < ant.path.size() - 1; ++i)
            {
                int u = getNodeIndex(ant.path[i].x, ant.path[i].y);
                int v = getNodeIndex(ant.path[i + 1].x, ant.path[i + 1].y);

                int idx1 = u * (MAP_WIDTH * MAP_HEIGHT) + v;
                int idx2 = v * (MAP_WIDTH * MAP_HEIGHT) + u;

                pheromones[idx1] += deposit;
                pheromones[idx2] += deposit;
            }
        }
    }

    void draw(sf::RenderWindow &window)
    {

        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            for (int x = 0; x < MAP_WIDTH; ++x)
            {
                int idx = (y * MAP_WIDTH + x) * 4;
                sf::Color color = obstacles[getNodeIndex(x, y)] ? sf::Color(50, 50, 50) : sf::Color::Red;
                if (x == startNode.x && y == startNode.y)
                    color = sf::Color::Green;
                else if (x == endNode.x && y == endNode.y)
                    color = sf::Color::Blue;

                gridVertices[idx].color = color;
                gridVertices[idx + 1].color = color;
                gridVertices[idx + 2].color = color;
                gridVertices[idx + 3].color = color;
            }
        }
        window.draw(gridVertices);

        if (!bestPath.empty())
        {
            pathVertices.clear();
            pathVertices.setPrimitiveType(sf::LineStrip);
            for (const auto &p : bestPath)
            {
                sf::Vector2f pos(p.x * CELL_SIZE + CELL_SIZE / 2.0f, p.y * CELL_SIZE + CELL_SIZE / 2.0f);
                pathVertices.append(sf::Vertex(pos, sf::Color::Yellow));
            }
            window.draw(pathVertices);
        }
    }

private:
    void initGraphics()
    {
        gridVertices.setPrimitiveType(sf::Quads);
        gridVertices.resize(MAP_WIDTH * MAP_HEIGHT * 4);

        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            for (int x = 0; x < MAP_WIDTH; ++x)
            {
                int idx = (y * MAP_WIDTH + x) * 4;
                float px = x * CELL_SIZE;
                float py = y * CELL_SIZE;

                gridVertices[idx].position = {px + 1, py + 1};
                gridVertices[idx + 1].position = {px + CELL_SIZE - 1, py + 1};
                gridVertices[idx + 2].position = {px + CELL_SIZE - 1, py + CELL_SIZE - 1};
                gridVertices[idx + 3].position = {px + 1, py + CELL_SIZE - 1};
            }
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * CELL_SIZE, MAP_HEIGHT * CELL_SIZE), "Ant Colony Optimization");
    window.setFramerateLimit(60);
    AntColonySimulation simulation;

    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();

            if (ev.type == sf::Event::MouseButtonPressed)
            {
                if (ev.mouseButton.button == sf::Mouse::Left)
                {
                    auto pos = sf::Mouse::getPosition(window);
                    int x = pos.x / CELL_SIZE;
                    int y = pos.y / CELL_SIZE;
                    simulation.setTarget(x, y);
                }
                else if (ev.mouseButton.button == sf::Mouse::Right)
                {
                    simulation.resetObstacles();
                }
            }
        }

        simulation.update();

        window.clear(sf::Color::Black);
        simulation.draw(window);
        window.display();
    }
    return 0;
}
