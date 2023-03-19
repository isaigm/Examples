#include <iostream>
#include <optional>
#include <SFML/Graphics.hpp>
#include <vector>
#define WIDTH 800
#define HEIGHT 600

namespace rod {
    struct PhysicsComponent
    {
        sf::Vector2f pos;
        sf::Vector2f vel;
    };
    struct RenderComponent
    {
        sf::CircleShape shape;
    };
    struct Entity
    {
        std::optional<PhysicsComponent> physics;
        std::optional<RenderComponent> render;
    };
};

namespace ge
{
    template<class EntityType>
    struct EntityManager
    {
        EntityManager(size_t n)
        {
            entities.reserve(n);
        }
        EntityType &createEntity()
        {
            return entities.emplace_back();
        }
        auto begin()
        {
            return entities.begin();
        }
        auto end()
        {
            return entities.end();
        }
    private:
        std::vector<EntityType> entities;
    };
};

namespace rod {
    using EntityManager = ge::EntityManager<rod::Entity>;

    struct PhysicsSystem
    {
        void update(EntityManager &em, float dt)
        {
            for(auto &e : em)
            {
                if(e.physics)
                {
                    e.physics->pos += dt * e.physics->vel;
                    e.render->shape.setPosition(e.physics->pos);
                }
            }
        }
    };

    struct RenderSystem
    {
    public:
        RenderSystem(sf::RenderTarget &renderTarget)  {
            rt = &renderTarget;
        }
        void update(EntityManager &em)
        {
            for(auto &e: em)
            {
                if(e.physics && e.render)
                {
                    rt->draw(e.render->shape);
                }
            }
        }
    private:
        sf::RenderTarget *rt;
    };
};

int main()
{

    rod::EntityManager entityManager(100);
    auto &e1 = entityManager.createEntity();
    e1.physics = rod::PhysicsComponent{ .pos{10, 0}, .vel{10, 10}};
    e1.render = rod::RenderComponent { sf::CircleShape(50)};

    auto &e2 = entityManager.createEntity();
    e2.physics = rod::PhysicsComponent{ .pos{100, 100}, .vel{20, 20}};
    e2.render = rod::RenderComponent { sf::CircleShape(100)};


    sf::RenderWindow window(sf::VideoMode(800, 600), "");
    window.setVerticalSyncEnabled(true);
    rod::RenderSystem render(window);
    rod::PhysicsSystem physics;
    sf::Clock clock;

    while(window.isOpen())
    {
        sf::Event ev;
        while(window.pollEvent(ev))
        {
            if(ev.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        float dt = clock.restart().asSeconds();
        window.clear();
        physics.update(entityManager, dt);
        render.update(entityManager);
        window.display();
    }
    return 0;
}
