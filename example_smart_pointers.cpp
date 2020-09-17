#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <thread>
#include <algorithm>
#define ROWS 35
#define COLS 40
#define CELL_WIDTH 15
#define CELL_HEIGHT 15
#define INIT_POSX 50
#define INIT_POSY 40
static int goal_x = 0;
static int goal_y = 0;
enum class CellType
{
    EMPTY,
    WALL,
    GOAL,
    START,
    STEP
};
float euclidean_distance(int x1, int y1, int x2, int y2)
{
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}
class Node
{
public:
    Node(int x, int y, float g = 0) : m_x(x), m_y(y)
    {
        h = euclidean_distance(x, y, goal_x, goal_y);
    }
    std::vector<std::unique_ptr<Node>> getNeighbours(CellType **map)
    {
        std::vector<std::unique_ptr<Node>> neighbours;
        for (int i = m_y - 1; i <= m_y + 1; ++i)
        {
            for (int j = m_x - 1; j <= m_x + 1; ++j)
            {
                if ((i == m_y && j == m_x) || i < 0 || i >= ROWS || j < 0 || j >= COLS)
                    continue;
                if (map[i][j] != CellType::WALL)
                {
                    auto node = std::make_unique<Node>(j, i);
                    node->m_parent = this;
                    node->g = g + 1;
                    neighbours.push_back(std::move(node));
                }
            }
        }
        return neighbours;
    }
    const int &getX() const
    {
        return m_x;
    }
    const int &getY() const
    {
        return m_y;
    }
    float getF()
    {
        return h + g;
    }
    float getLevel()
    {
        return g;
    }
    Node *getParent() const
    {
        return m_parent;
    }

private:
    Node *m_parent = nullptr;
    int m_x, m_y;
    float h, g;
};
std::pair<int, int> convert_coords_to_idx(sf::Vector2i &pos)
{
    int r = floor((pos.y - INIT_POSY) / CELL_HEIGHT);
    int c = std::floor((pos.x - INIT_POSX) / CELL_WIDTH);
    return std::make_pair(r, c);
}
bool set_cell_at(CellType **map, sf::Vector2i &pos, CellType ctype)
{
    auto [r, c] = convert_coords_to_idx(pos);
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
    {
        map[r][c] = ctype;
        return true;
    }
    return false;
}
void a_star(CellType **map, int start_x, int start_y)
{
    auto search = [](const std::vector<std::unique_ptr<Node>> &nodes, int x, int y) {
        size_t i = 0;
        for (; i < nodes.size(); i++)
        {
            if (x == nodes[i]->getX() && y == nodes[i]->getY())
            {
                return std::make_pair(true, i);
            }
        }
        return std::make_pair(false, i);
    };
    std::vector<std::unique_ptr<Node>> open_list;
    std::vector<std::unique_ptr<Node>> closed_list;
    open_list.push_back(std::make_unique<Node>(start_x, start_y));
    bool found = false;
    while (open_list.size() > 0)
    {
        auto curr_node = open_list[0].get();
        if (curr_node->getX() == goal_x && curr_node->getY() == goal_y)
        {
            found = true;
            Node *aux = curr_node->getParent();
            while (aux->getParent() != nullptr)
            {
                map[aux->getY()][aux->getX()] = CellType::STEP;
                aux = aux->getParent();
            }
            break;
        }
        auto neighbours = curr_node->getNeighbours(map);
        for (size_t i = 0; i < neighbours.size(); i++)
        {
            auto in_closed = search(closed_list, neighbours[i]->getX(), neighbours[i]->getY());
            auto in_open = search(open_list, neighbours[i]->getX(), neighbours[i]->getY());
            if (in_closed.first)
                continue;
            else if (in_open.first)
            {
                if (open_list[in_open.second]->getLevel() <= neighbours[i]->getLevel())
                {
                    continue;
                }
            }
            open_list.push_back(std::move(neighbours[i]));
        }
        closed_list.push_back(std::move(open_list[0]));
        open_list.erase(open_list.begin());
        std::sort(open_list.begin(), open_list.end(), [](const std::unique_ptr<Node> &n1, const std::unique_ptr<Node> &n2) {
            return n1->getF() < n2->getF();
        });
    }
    if (!found)
    {
        std::cout << "unreachable target\n";
    }
}
int main()
{
    CellType **map = new CellType *[ROWS];
    for (int i = 0; i < ROWS; i++)
    {
        map[i] = new CellType[COLS];
    }
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            map[i][j] = CellType::EMPTY;
        }
    }
    sf::RenderWindow window(sf::VideoMode(800, 600), "A*");
    sf::Event ev;
    window.setVerticalSyncEnabled(true);
    bool can_draw = false;
    bool goal = false;
    bool start = false;
    bool new_path = true;
    int start_x = 0;
    int start_y = 0;
    while (window.isOpen())
    {
        while (window.pollEvent(ev))
        {
            switch (ev.type)
            {
            case sf::Event::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    can_draw = !can_draw;
                }
                else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                {
                    auto pos = sf::Mouse::getPosition(window);
                    if (set_cell_at(map, pos, CellType::START) && !start && new_path)
                    {
                        auto [r, c] = convert_coords_to_idx(pos);
                        start_y = r;
                        start_x = c;
                        start = true;
                        std::cout << start_x << ", " << start_y << std::endl;
                    }
                    else if (set_cell_at(map, pos, CellType::GOAL) && !goal && new_path)
                    {
                        auto [r, c] = convert_coords_to_idx(pos);
                        goal_y = r;
                        goal_x = c;
                        goal = true;
                        std::cout << goal_x << ", " << goal_y << std::endl;
                    }
                }
                break;
            case sf::Event::MouseMoved:
            {
                if (can_draw && new_path)
                {
                    auto pos = sf::Mouse::getPosition(window);
                    set_cell_at(map, pos, CellType::WALL);
                }
                break;
            }
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Enter && goal && start && new_path)
                {
                    std::thread th(a_star, map, start_x, start_y);
                    th.join();
                    goal = false;
                    start = false;
                    new_path = false;
                }
                else if (ev.key.code == sf::Keyboard::R)
                {
                    for (int i = 0; i < ROWS; i++)
                    {
                        for (int j = 0; j < COLS; j++)
                        {
                            map[i][j] = CellType::EMPTY;
                        }
                    }
                    new_path = true;
                }
                break;
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }
        window.clear(sf::Color::White);
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                sf::RectangleShape rect;
                rect.setSize({CELL_WIDTH, CELL_HEIGHT});
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(1.0f);
                rect.setPosition(INIT_POSX + j * CELL_WIDTH, INIT_POSY + i * CELL_HEIGHT);
                switch (map[i][j])
                {
                case CellType::EMPTY:
                    rect.setFillColor(sf::Color::White);
                    break;
                case CellType::WALL:
                    rect.setFillColor(sf::Color::Black);
                    break;
                case CellType::GOAL:
                    rect.setFillColor(sf::Color::Red);
                    break;
                case CellType::START:
                    rect.setFillColor(sf::Color::Blue);
                    break;
                case CellType::STEP:
                    rect.setFillColor(sf::Color::Green);
                    break;
                default:
                    break;
                }
                window.draw(rect);
            }
        }
        window.display();
    }
    for (int i = 0; i < ROWS; i++)
    {
        delete[] map[i];
    }
    delete[] map;
    return 0;
}
