#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <map>
#include <sstream>
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define DIM 80
static const float start_pheromones = 0.1f;
static const float rho = 0.6f;
bool visited[MAP_HEIGHT][MAP_WIDTH];
bool map[MAP_HEIGHT][MAP_WIDTH];
static const int n_ants = 50;
static const float q = 10;
static float alpha = 1;
static float beta = 1;
struct edge
{
    float pheremones = start_pheromones;
    float h = 1;
};
std::map<std::string, edge> all_edges;

std::string get_key(sf::Vector2i origin, sf::Vector2i neighbour)
{
    char buff[256];
    snprintf(buff, sizeof(buff), "%d,%d,%d,%d", origin.x, origin.y, neighbour.x, neighbour.y);
    std::string key(buff);
    if (all_edges.find(key) != all_edges.end())
        return key;
    snprintf(buff, sizeof(buff), "%d,%d,%d,%d", neighbour.x, neighbour.y, origin.x, origin.y);
    return std::string(buff);
}
float get_dist(sf::Vector2i start, sf::Vector2i end)
{
    float dx = float(start.x - end.x);
    float dy = float(start.y - end.y);
    return std::sqrt(dx * dx + dy * dy);
}
struct ant
{
    float total_dist()
    {
        float dist = 0;
        for (int i = 0; i < int(path.size() - 1); i++)
        {
            dist += get_dist(path[i], path[i + 1]);
        }
        return dist;
    }
    void update()
    {
        float dist = total_dist();
        for (int i = 0; i < int(path.size() - 1); i++)
        {
            auto key = get_key(path[i], path[i + 1]);
            all_edges[key].pheremones += q / dist;
        }
    }
    std::vector<sf::Vector2i> path;
};
ant best_ant;

std::vector<sf::Vector2i> get_next_moves(int i, int j)
{
    std::vector<sf::Vector2i> next_moves;
    for (int y = i - 1; y <= i + 1; y++)
    {
        for (int x = j - 1; x <= j + 1; x++)
        {
            if ((x == j && y == i) || x < 0 || y < 0 || y >= MAP_HEIGHT || x >= MAP_WIDTH || visited[y][x] || !map[y][x])
                continue;
            next_moves.push_back({x, y});
        }
    }
    return next_moves;
}

void restart_visited()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            visited[i][j] = false;
        }
    }
}
void init()
{
    srand(time(nullptr));
    restart_visited();
    best_ant.path.erase(best_ant.path.begin(), best_ant.path.end());
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            auto n = get_next_moves(i, j);
            for (auto &pos : n)
            {
                auto key = get_key({j, i}, pos);
                all_edges[key].h = get_dist(pos, {j, i});
                all_edges[key].pheremones = start_pheromones;
            }
        }
    }
}
float get_rand()
{
    return float(rand()) / float(RAND_MAX);
}

void ACO(sf::Vector2i start, sf::Vector2i end, sf::RenderTexture &rt)
{
    rt.clear();
    std::vector<ant> ants;

    for (int i = 0; i < n_ants; i++)
    {
        ants.emplace_back();
        auto curr_node = start;

        while (true)
        {
            auto next_moves = get_next_moves(curr_node.y, curr_node.x);
            ants.back().path.push_back(curr_node);
            float sum = 0;
            float prob = -1;
            float last_prob = 0;
            if (curr_node.x == end.x && curr_node.y == end.y)
            {
                float best_dist = best_ant.total_dist();
                if (best_dist == 0 || best_dist > ants.back().total_dist())
                {
                    std::cout << "best path found\n";
                    best_ant.path = ants.back().path;
                }
                break;
            }
            if (next_moves.size() == 0)
            {
                ants.pop_back();
                break;
            }
            for (auto &next_move : next_moves)
            {
                auto key = get_key(curr_node, next_move);
                auto &curr_edge = all_edges[key];
                sum += std::pow(curr_edge.pheremones, alpha) * std::pow(1.0f / curr_edge.h, beta);
            }
            float r = get_rand();
            int index_of_next_move = rand() % (next_moves.size());
            for (int i = 0; i < next_moves.size(); i++)
            {
                auto next_move = next_moves[i];
                auto key = get_key(curr_node, next_move);
                auto curr_edge = all_edges[key];
                float probability = std::pow(curr_edge.pheremones, alpha) * std::pow(1.0f / curr_edge.h, beta) / sum;
                if (prob == -1)
                    prob = probability;
                else
                    prob += probability;
                if (r > last_prob && r <= prob)
                {
                    index_of_next_move = i;
                    break;
                }
                last_prob = probability;
            }
            visited[curr_node.y][curr_node.x] = true;
            curr_node = next_moves[index_of_next_move];
        }
        restart_visited();
    }
    for (auto &e : all_edges)
    {
        e.second.pheremones = (1 - rho) * e.second.pheremones;
    }
    for (auto &a : ants)
    {
        a.update();
    }
    best_ant.update();
    for (int i = 0; i < int(best_ant.path.size() - 1); i++)
    {
        auto key = get_key(best_ant.path[i], best_ant.path[i + 1]);
        std::stringstream ss(key);
        sf::Vector2i curr, next;
        char del;
        ss >> curr.x;
        ss >> del;
        ss >> curr.y;
        ss >> del;
        ss >> next.x;
        ss >> del;
        ss >> next.y;
        sf::Vector2f p, q;
        p.x = curr.x * DIM + DIM / 2.0f;
        p.y = curr.y * DIM + DIM / 2.0f;
        q.x = next.x * DIM + DIM / 2.0f;
        q.y = next.y * DIM + DIM / 2.0f;
        sf::Vertex line[2];
        line[0].position = p;
        line[0].color = sf::Color(255, 255, 0);
        line[1].position = q;
        line[1].color = line[0].color;
        rt.draw(line, 2, sf::LineStrip);
    }
    rt.display();
}
int main()
{
    sf::RenderTexture rt;
    rt.create(800, 800);

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            map[i][j] = true;
        }
    }
    for (int i = 0; i < 20; i++)
    {
        int rx = rand() % MAP_WIDTH;
        int ry = rand() % MAP_HEIGHT;
        map[ry][rx] = false;
    }
    sf::Sprite sprite;
    sprite.setTexture(rt.getTexture());
    sf::RenderWindow window(sf::VideoMode(800, 800), "");
    // window.setVerticalSyncEnabled(true);
    int i = 9;
    int j = 9;
    init();

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
            else if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
            {
                auto mouse_pos = sf::Mouse::getPosition(window);
                i = mouse_pos.y / DIM;
                j = mouse_pos.x / DIM;
                init();
            }
        }

        ACO({0, 0}, {j, i}, rt);

        window.clear(sf::Color::Black);
        window.draw(sprite);
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            for (int j = 0; j < MAP_WIDTH; j++)
            {
                sf::RectangleShape rect;
                rect.setSize({DIM, DIM});
                sf::Vector2f pos{float(j * DIM), float(i * DIM)};
                rect.setFillColor(sf::Color::Transparent);
                rect.setOutlineThickness(0.7f);
                rect.setPosition(pos);

                if (!map[i][j])
                {
                    rect.setFillColor(sf::Color::Red);
                }
                window.draw(rect);
            }
        }
        window.display();
    }
    return 0;
}
