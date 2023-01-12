#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
static const int WIDTH = 800;
static const int HEIGHT = 600;
static const int K = 15;
static const int CIRCLE_RADIUS = 5;
sf::Vector2i mousePos{0, 0};

enum class Category
{
    Rectangle,
    Circle
};
class point
{
public:
    point(sf::Vector2f pos, Category category = Category::Circle)
    {
        this->category = category;
        this->pos = pos;
    }
    Category getCategory()
    {
        return category;
    }
    float distTo(sf::Vector2f otherPos)
    {
        float diffx = pos.x - otherPos.x;
        float diffy = pos.y - otherPos.y;

        return sqrt(diffx * diffx + diffy * diffy);
    }
    sf::Vector2f getPos()
    {
        return pos;
    }
    void setShowBoundingBox(bool val)
    {
        showBoundingBox = val;
    }
    void render(sf::RenderTarget &rt)
    {

        if (category == Category::Circle)
        {
            sf::CircleShape circle(CIRCLE_RADIUS);
            circle.setOrigin({CIRCLE_RADIUS, CIRCLE_RADIUS});
            circle.setPosition(pos);
            circle.setFillColor(sf::Color::Green);
            rt.draw(circle);
        }
        else if (category == Category::Rectangle)
        {
            sf::RectangleShape rect;
            rect.setSize({CIRCLE_RADIUS, CIRCLE_RADIUS});
            rect.setFillColor(sf::Color::Red);
            rect.setPosition({pos.x - CIRCLE_RADIUS / 2, pos.y - CIRCLE_RADIUS / 2});
            rt.draw(rect);
        }
        if (showBoundingBox)
        {
            sf::RectangleShape rect;
            float size = 17;
            rect.setSize({size, size});
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color::Blue);
            rect.setOutlineThickness(0.4f);
            rect.setPosition({pos.x - size / 2, pos.y - size / 2});
            rt.draw(rect);
        }
    }
private:
    Category category;
    sf::Vector2f pos;
    bool showBoundingBox = false;
};
int getRandomNumber(int min, int max)
{
    if (min == max)
        return min;
    return rand() % (max - min) + min;
}
void populate(std::vector<point> &dataSet)
{
    int nCircles = 20;
    int nRectangles = 35;
    sf::Vector2f top{0, 0};
    sf::Vector2f bottom{WIDTH, HEIGHT};
    float m = (bottom.y - top.y) / (bottom.x - top.x);
    for (int i = 0; i < nCircles; i++)
    {
        int x = getRandomNumber(CIRCLE_RADIUS, WIDTH - CIRCLE_RADIUS);
        int y = getRandomNumber(int(m * x), HEIGHT - CIRCLE_RADIUS);
        sf::Vector2f pos{float(x), float(y)};
        dataSet.push_back(point(pos, Category::Circle));
    }
    for (int i = 0; i < nRectangles; i++)
    {
        int x = getRandomNumber(CIRCLE_RADIUS, WIDTH - CIRCLE_RADIUS);
        int y = getRandomNumber(CIRCLE_RADIUS, int(m * x));
        sf::Vector2f pos{float(x), float(y)};
        dataSet.push_back(point(pos, Category::Rectangle));
    }
}
class comparator
{
public:
    bool operator()(point *p1, point *p2)
    {
        return p1->distTo((sf::Vector2f)mousePos) < p2->distTo((sf::Vector2f)mousePos);
    }
};
void knn(std::vector<point> &dataSet)
{
    std::priority_queue<point *, std::vector<point *>, comparator> heap;
    for (auto &point : dataSet)
    {
        heap.push(&point);
        point.setShowBoundingBox(false);
        if (heap.size() > K)
        {
            heap.pop();
        }
    }
    int circlesNeighbours = 0;
    int rectangleNeighbours = 0;
    while (heap.size() > 0)
    {
        auto currPoint = heap.top();
        currPoint->setShowBoundingBox(true);
        if (currPoint->getCategory() == Category::Rectangle)
            rectangleNeighbours++;
        else if (currPoint->getCategory() == Category::Circle)
            circlesNeighbours++;
        heap.pop();
    }
    printf("Circles: %d, Rectangles: %d\n", circlesNeighbours, rectangleNeighbours);
    if (circlesNeighbours > rectangleNeighbours)
    {
        dataSet.push_back(point((sf::Vector2f)mousePos, Category::Circle));
    }
    else if (rectangleNeighbours > circlesNeighbours)
    {
        dataSet.push_back(point((sf::Vector2f)mousePos, Category::Rectangle));
    }
    else
    {
        printf("Misma cantidad de vecinos\n");
    }
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "KNN");
    std::vector<point> dataSet;
    window.setVerticalSyncEnabled(true);
    srand(time(nullptr));
    populate(dataSet);

    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                mousePos = sf::Mouse::getPosition(window);
                knn(dataSet);
            }
            else if (ev.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        window.clear();
        for (auto &sample : dataSet)
        {
            sample.render(window);
        }
        window.display();
    }
    return 0;
}
