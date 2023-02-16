#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <assert.h>
static const int WIDTH = 800;
static const int HEIGHT = 600;
static const int K = 4;
static const int R = 10;
static const int N = 25;
class point
{
public:
    point(sf::Vector2f pos, sf::Color color = sf::Color::Blue) : mPos(pos), mColor(color)
    {
    }
    auto getPos()
    {
        return mPos;
    }
    void render(sf::RenderTarget &rt)
    {
        sf::CircleShape circle(R);
        circle.setFillColor(mColor);
        circle.setOrigin({R, R});
        circle.setPosition(mPos);
        rt.draw(circle);
    }
    void setColor(sf::Color color)
    {
        mColor = color;
    }
    void setCluster(int cluster)
    {
        mIndexCluster = cluster;
    }

private:
    sf::Color mColor;
    sf::Vector2f mPos;
    int mIndexCluster = -1;
};
class centroid
{
public:
    centroid(sf::Vector2f pos) : mPos(pos)
    {
    }
    void reset()
    {
        points.erase(points.begin(), points.end());
    }
    const std::vector<int> &getPoints()
    {
        return points;
    }
    void addPoint(int p)
    {
        points.push_back(p);
    }
    void render(sf::RenderTarget &rt)
    {
        sf::CircleShape circle(R);
        circle.setFillColor(sf::Color::Red);
        circle.setOrigin({R, R});
        circle.setPosition(mPos);
        rt.draw(circle);
    }
    void setPos(sf::Vector2f pos)
    {
        mPos = pos;
    }
    sf::Vector2f getPos()
    {
        return mPos;
    }

private:
    std::vector<int> points;
    sf::Vector2f mPos;
};
int getRandomInt(int min, int max)
{
    return rand() % (max - min) + min;
}
void setCentroids(std::vector<point> &dataSet, std::vector<centroid> &centroids)
{
    for (auto &c : centroids)
        c.reset();
    for (int j = 0; j < dataSet.size(); j++)
    {
        float minDist = 1e16;
        int cluster = -1;
        auto &p = dataSet[j];
        for (int i = 0; i < centroids.size(); i++)
        {
            float dx = p.getPos().x - centroids[i].getPos().x;
            float dy = p.getPos().y - centroids[i].getPos().y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < minDist)
            {
                minDist = dist;
                cluster = i;
            }
        }
        assert(cluster != -1);
        switch (cluster)
        {
        case 0:
            p.setColor(sf::Color::Green);
            break;
        case 1:
            p.setColor(sf::Color::Magenta);
            break;
        case 2:
            p.setColor(sf::Color::Cyan);
            break;
        case 3:
            p.setColor(sf::Color::Yellow);
            break;
        default:
            break;
        }
        centroids[cluster].addPoint(j);
    }
    for (auto &c : centroids)
    {
        sf::Vector2f newPos{0, 0};
        for (auto &p : c.getPoints())
        {
            newPos.x += dataSet[p].getPos().x;
            newPos.y += dataSet[p].getPos().y;
        }
        newPos.x /= float(c.getPoints().size());
        newPos.y /= float(c.getPoints().size());
        c.setPos(newPos);
    }
}

int main(int argc, char **argv)
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "K means");
    window.setFramerateLimit(1);
    srand(time(nullptr));
    std::vector<centroid> centroids;
    std::vector<point> dataSet;
    for (int i = 0; i < K; i++)
    {
        float x = getRandomInt(R, WIDTH - R);
        float y = getRandomInt(R, HEIGHT - R);
        centroids.push_back(centroid({x, y}));
    }
    for (int i = 0; i < N; i++)
    {
        float x = getRandomInt(R, WIDTH - R);
        float y = getRandomInt(R, HEIGHT - R);
        dataSet.push_back(point({x, y}));
    }
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

        setCentroids(dataSet, centroids);
        window.clear();
        for (auto &c : centroids)
        {
            c.render(window);
        }
        for (auto &p : dataSet)
        {
            p.render(window);
        }
        window.display();
    }
    return 0;
}
