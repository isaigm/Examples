#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#define SIZE 1000
#define RADIUS 5
class particle;
struct node;
sf::Color colors[] = {sf::Color::Blue, sf::Color::Red, sf::Color::Green};
std::string get_uuid()
{
    std::string uuid = "";
    auto gen_random_chars = [](int n) -> std::string
    {
        std::string res = "";
        for (int i = 0; i < n; i++)
        {
            char buff[2];
            int random = rand() % 16;
            snprintf(buff, sizeof(buff), "%x", random);
            res += buff;
        }
        return res;
    };
    uuid += gen_random_chars(8) + "-";
    uuid += gen_random_chars(4) + "-";
    uuid += gen_random_chars(4) + "-";
    uuid += gen_random_chars(4) + "-";
    uuid += gen_random_chars(12);
    return uuid;
}

class particle
{
public:
    particle()
    {
        shape.setRadius(RADIUS);
        shape.setOrigin({RADIUS, RADIUS});
        shape.setFillColor(sf::Color::Blue);
    }
    particle(float x, float y, float xxspeed, float yyspeed) : particle()
    {
        set_pos(x, y);
        set_uuid();
        xspeed = xxspeed;
        yspeed = yyspeed;
    }
    void set_pos(float x, float y)
    {
        shape.setPosition(x, y);
    }
    sf::Vector2f get_pos()
    {
        return shape.getPosition();
    }
    void update(float dt)
    {
        shape.move({dt * xspeed, dt * yspeed});
    }
    std::string get_id()
    {
        return id;
    }

    void set_uuid()
    {
        id = get_uuid();
    }
    sf::CircleShape get_shape()
    {
        return shape;
    }
    bool collides(sf::RectangleShape &rect)
    {
        return shape.getGlobalBounds().intersects(rect.getGlobalBounds());
    }
    bool collides(particle &other)
    {
        return shape.getGlobalBounds().intersects(other.get_shape().getGlobalBounds());
    }
    void out_of_bounds()
    {
        auto pos = get_pos();
        if (pos.x < RADIUS || pos.x > SIZE - RADIUS)
        {
            xspeed *= -1;
        }
        if (pos.y < RADIUS || pos.y > SIZE - RADIUS)
        {
            yspeed *= -1;
        }
    }
    void render(sf::RenderTarget &rt)
    {
        rt.draw(shape);
    }
    void change_direction()
    {
        xspeed *= -1;
        yspeed *= -1;
    }

private:
    sf::CircleShape shape;
    std::string id = "default";
    float xspeed = 0, yspeed = 0;
};
struct node
{
    node *topleft = nullptr;
    node *topright = nullptr;
    node *bottomleft = nullptr;
    node *bottomright = nullptr;
    sf::RectangleShape bounds;
};
class quadtree
{
public:
    quadtree()
    {
        create(&root, SIZE, SIZE / 2, SIZE / 2, 0, 4);
    }
    void render(sf::RenderTarget &rt)
    {
        render(root, rt);
        for (auto &n : leaf_nodes)
        {
            for (auto &p : n.second)
            {
                p.render(rt);
            }
        }
    }
    void add_particle(float x, float y, float xspeed, float yspeed)
    {
        node *parent = search_for_parent(root, x, y);
        if (parent != nullptr)
        {
            leaf_nodes[parent].push_back(particle(x, y, xspeed, yspeed));
        }
    }
    void update(float dt)
    {
        auto search_and_remove = [](std::string uuid, std::vector<particle> &particles)
        {
            for (int i = 0; i < particles.size(); i++)
            {
                if (particles[i].get_id() == uuid)
                {
                    particles.erase(particles.begin() + i);
                    break;
                }
            }
        };

        for (auto &n : leaf_nodes)
        {
            for (int i = 0; i < n.second.size(); i++)
            {
                for (int j = 0; j < n.second.size(); j++)
                {
                    if (i != j && n.second[i].collides(n.second[j]))
                    {
                        n.second[i].change_direction();
                        n.second[j].change_direction();
                        n.second[i].update(dt);
                        n.second[j].update(dt);
                    }
                }
            }
        }
        for (auto &n : leaf_nodes)
        {
            std::vector<std::pair<std::string, std::vector<particle> &>> toremove;
            for (int i = 0; i < n.second.size(); i++)
            {
                auto &p = n.second[i];
                p.update(dt);
                p.out_of_bounds();
                node *parent = search_for_parent(root, p.get_pos().x, p.get_pos().y);
                if (parent != nullptr && parent != n.first)
                {
                    std::pair<std::string, std::vector<particle> &> pair{p.get_id(), n.second};

                    leaf_nodes[parent].push_back(p);
                    toremove.push_back(pair);
                }
            }
            for (int i = 0; i < toremove.size(); i++)
            {
                search_and_remove(toremove[i].first, toremove[i].second);
            }
        }
    }

private:
    std::map<node *, std::vector<particle>> leaf_nodes;
    node *root;
    node *search_for_parent(node *curr_root, float x, float y)
    {
        if (curr_root != nullptr)
        {
            particle p;
            p.set_pos(x, y);
            if (p.collides(curr_root->bounds) && curr_root->topleft == nullptr && curr_root->topright == nullptr && curr_root->bottomleft == nullptr && curr_root->bottomright == nullptr)
            {
                return curr_root;
            }
            node *n = nullptr;
            if (curr_root->topleft != nullptr && p.collides(curr_root->topleft->bounds))
            {
                n = search_for_parent(curr_root->topleft, x, y);
                return n;
            }
            if (curr_root->topright != nullptr && p.collides(curr_root->topright->bounds))
            {
                n = search_for_parent(curr_root->topright, x, y);
                return n;
            }
            if (curr_root->bottomleft != nullptr && p.collides(curr_root->bottomleft->bounds))
            {
                n = search_for_parent(curr_root->bottomleft, x, y);
                return n;
            }
            if (curr_root->bottomright != nullptr && p.collides(curr_root->bottomright->bounds))
            {
                n = search_for_parent(curr_root->bottomright, x, y);
                return n;
            }
        }
        return nullptr;
    }
    void render(node *curr_root, sf::RenderTarget &rt)
    {
        if (curr_root != nullptr)
        {
            rt.draw(curr_root->bounds);
            render(curr_root->topleft, rt);
            render(curr_root->topright, rt);
            render(curr_root->bottomleft, rt);
            render(curr_root->bottomright, rt);
        }
    }
    void create(node **curr_root, float size, float x, float y, int curr_depth, int max_depth)
    {
        if (curr_depth <= max_depth)
        {
            *curr_root = new node;
            sf::Vector2f coords{x - size / 2, y - size / 2};
            (*curr_root)->bounds.setSize({size, size});
            (*curr_root)->bounds.setPosition(coords);
            (*curr_root)->bounds.setFillColor(sf::Color::Transparent);
            (*curr_root)->bounds.setOutlineColor(sf::Color::Black);
            (*curr_root)->bounds.setOutlineThickness(1.5f);
            size /= 2;
            create(&((*curr_root)->topleft), size, x - size / 2, y - size / 2, curr_depth + 1, max_depth);
            create(&((*curr_root)->topright), size, x + size / 2, y - size / 2, curr_depth + 1, max_depth);
            create(&((*curr_root)->bottomleft), size, x - size / 2, y + size / 2, curr_depth + 1, max_depth);
            create(&((*curr_root)->bottomright), size, x + size / 2, y + size / 2, curr_depth + 1, max_depth);
        }
    }
};
int main()
{
    srand(time(nullptr));
    quadtree qt;
    sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "QUADTREE");
    window.setFramerateLimit(60);
    for (int i = 0; i < 1000; i++)
    {
        int x = rand() % (SIZE - 2 * RADIUS) + RADIUS;
        int y = rand() % (SIZE - 2 * RADIUS) + RADIUS;
        int xspeed = rand() % 50;
        int yspeed = rand() % 50;
        if (rand() % 2)
            xspeed *= -1;
        if (rand() % 2)
            yspeed *= -1;
        qt.add_particle(x, y, xspeed, yspeed);
    }

    sf::Clock clock;
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
        float dt = clock.restart().asSeconds();
        std::cout << 1 / dt << "\n";
        qt.update(dt);
        window.clear(sf::Color::White);
        qt.render(window);

        window.display();
    }
    return 0;
}
