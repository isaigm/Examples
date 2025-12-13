#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr float RADIUS = 5.0f;
constexpr int MAX_PARTICLES_PER_NODE = 8;
constexpr int MAX_DEPTH = 6;
constexpr int PARTICLE_COUNT = 1000;

class Particle {
public:
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Color color;

    Particle(float x, float y, float vx, float vy)
        : pos(x, y), vel(vx, vy), color(sf::Color::Blue) {
    }

    void update(float dt) {
        pos += vel * dt;
    }

    // --- CAMBIO 2: Límites independientes para X e Y ---
    void resolve_bounds() {
        // Eje X (Ancho)
        if (pos.x < RADIUS) {
            pos.x = RADIUS;
            vel.x *= -1;
        }
        else if (pos.x > WINDOW_WIDTH - RADIUS) {
            pos.x = WINDOW_WIDTH - RADIUS;
            vel.x *= -1;
        }

        // Eje Y (Alto)
        if (pos.y < RADIUS) {
            pos.y = RADIUS;
            vel.y *= -1;
        }
        else if (pos.y > WINDOW_HEIGHT - RADIUS) {
            pos.y = WINDOW_HEIGHT - RADIUS;
            vel.y *= -1;
        }
    }
};

struct Node {
    sf::FloatRect bounds;
    std::vector<Particle*> particles;
    std::unique_ptr<Node> children[4];
    bool is_leaf = true;
    int depth;

    Node(sf::FloatRect b, int d) : bounds(b), depth(d) {
        particles.reserve(MAX_PARTICLES_PER_NODE);
    }
};

class Quadtree {
private:
    std::unique_ptr<Node> root;
    sf::FloatRect initial_bounds;

public:
    Quadtree(float width, float height) {
        initial_bounds = sf::FloatRect(0, 0, width, height);
        clear();
    }

    void clear() {
        root = std::make_unique<Node>(initial_bounds, 0);
    }

    void insert(Particle* p) {
        insert_impl(root.get(), p);
    }

    void query_and_resolve(Particle* p) {
        query_impl(root.get(), p);
    }

    void render_bounds(sf::RenderTarget& rt) {
        render_impl(root.get(), rt);
    }

private:
    void insert_impl(Node* node, Particle* p) {
        if (!node->bounds.contains(p->pos)) return;

        if (node->is_leaf) {
            if (node->particles.size() < MAX_PARTICLES_PER_NODE || node->depth >= MAX_DEPTH) {
                node->particles.push_back(p);
            }
            else {
                split(node);
                insert_impl(node, p);
            }
        }
        else {
            int index = get_index(node, p->pos);
            if (index != -1) insert_impl(node->children[index].get(), p);
        }
    }

    void split(Node* node) {
        node->is_leaf = false;
        // Al dividir, simplemente cortamos el rectángulo actual a la mitad.
        // Si el nodo es rectangular, los hijos también lo serán.
        float w = node->bounds.width / 2.0f;
        float h = node->bounds.height / 2.0f;
        float x = node->bounds.left;
        float y = node->bounds.top;

        node->children[0] = std::make_unique<Node>(sf::FloatRect(x, y, w, h), node->depth + 1);
        node->children[1] = std::make_unique<Node>(sf::FloatRect(x + w, y, w, h), node->depth + 1);
        node->children[2] = std::make_unique<Node>(sf::FloatRect(x, y + h, w, h), node->depth + 1);
        node->children[3] = std::make_unique<Node>(sf::FloatRect(x + w, y + h, w, h), node->depth + 1);

        for (Particle* existing : node->particles) {
            int index = get_index(node, existing->pos);
            if (index != -1) insert_impl(node->children[index].get(), existing);
        }
        node->particles.clear();
    }

    int get_index(Node* node, const sf::Vector2f& pos) {
        float midX = node->bounds.left + node->bounds.width / 2.0f;
        float midY = node->bounds.top + node->bounds.height / 2.0f;
        bool top = (pos.y < midY);
        bool left = (pos.x < midX);
        if (left) return top ? 0 : 2;
        else return top ? 1 : 3;
    }

    void query_impl(Node* node, Particle* p) {
        if (!node) return;

        for (Particle* other : node->particles) {
            if (p == other) continue;

            float dx = p->pos.x - other->pos.x;
            float dy = p->pos.y - other->pos.y;
            float distSq = dx * dx + dy * dy;
            float minDist = RADIUS * 2;

            if (distSq < minDist * minDist && distSq > 0.0001f) {
                resolve_collision(p, other, dx, dy, distSq);
            }
        }

        if (!node->is_leaf) {
            int index = get_index(node, p->pos);
            if (index != -1) {
                query_impl(node->children[index].get(), p);
            }
        }
    }

    void resolve_collision(Particle* p1, Particle* p2, float dx, float dy, float distSq) {
        float dist = std::sqrt(distSq);

        float overlap = (RADIUS * 2) - dist;
        float nx = dx / dist;
        float ny = dy / dist;

        p1->pos.x += nx * overlap * 0.5f;
        p1->pos.y += ny * overlap * 0.5f;
        p2->pos.x -= nx * overlap * 0.5f;
        p2->pos.y -= ny * overlap * 0.5f;

        float tx = -ny;
        float ty = nx;

        float dpTan1 = p1->vel.x * tx + p1->vel.y * ty;
        float dpTan2 = p2->vel.x * tx + p2->vel.y * ty;

        float dpNorm1 = p1->vel.x * nx + p1->vel.y * ny;
        float dpNorm2 = p2->vel.x * nx + p2->vel.y * ny;

        float m1 = 1.0f;
        float m2 = 1.0f;

        float mom1 = (dpNorm1 * (m1 - m2) + 2.0f * m2 * dpNorm2) / (m1 + m2);
        float mom2 = (dpNorm2 * (m2 - m1) + 2.0f * m1 * dpNorm1) / (m1 + m2);

        p1->vel.x = tx * dpTan1 + nx * mom1;
        p1->vel.y = ty * dpTan1 + ny * mom1;
        p2->vel.x = tx * dpTan2 + nx * mom2;
        p2->vel.y = ty * dpTan2 + ny * mom2;

        p1->color = sf::Color::Red;
        p2->color = sf::Color::Red;
    }

    void render_impl(Node* node, sf::RenderTarget& rt) {
        if (!node) return;

        if (!node->is_leaf || !node->particles.empty()) {
            sf::RectangleShape rect;
            rect.setPosition(node->bounds.left, node->bounds.top);
            rect.setSize({ node->bounds.width, node->bounds.height });
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color(50, 50, 50, 100));
            rect.setOutlineThickness(1.0f);
            rt.draw(rect);
        }

        if (!node->is_leaf) {
            for (auto& child : node->children) render_impl(child.get(), rt);
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // --- CAMBIO 3: Usar las nuevas dimensiones en la ventana ---
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Rectangular Quadtree");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    particles.reserve(PARTICLE_COUNT);

    for (int i = 0; i < PARTICLE_COUNT; i++) {
        // --- CAMBIO 4: Generación aleatoria respetando el rectángulo ---
        float x = rand() % (WINDOW_WIDTH - (int)RADIUS * 2) + RADIUS;
        float y = rand() % (WINDOW_HEIGHT - (int)RADIUS * 2) + RADIUS;

        float vx = (rand() % 100 - 50) * 2.0f;
        float vy = (rand() % 100 - 50) * 2.0f;
        particles.emplace_back(x, y, vx, vy);
    }

    // --- CAMBIO 5: Inicializar Quadtree con dimensiones rectangulares ---
    Quadtree qt(WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::CircleShape circle(RADIUS);
    circle.setOrigin(RADIUS, RADIUS);

    sf::Clock clock;
    bool show_grid = false;

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Space)
                show_grid = !show_grid;
        }

        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;

        qt.clear();
        for (auto& p : particles) {
            p.color = sf::Color::Blue;
            qt.insert(&p);
        }

        for (auto& p : particles) {
            p.update(dt);
            p.resolve_bounds();
            qt.query_and_resolve(&p);
        }

        window.clear(sf::Color::White);

        if (show_grid) qt.render_bounds(window);

        for (const auto& p : particles) {
            circle.setPosition(p.pos);
            circle.setFillColor(p.color);
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}
