#include <SFML/Graphics.hpp>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <set>
#include <fstream>
#include <sstream>

enum class Type
{
    Noise = -1,
    Border,
    Core
};
struct label
{
    label(Type t = Type::Noise) : type(t) {}
    Type type;
    int cluster{};
};

using Point = Eigen::Vector2d;

void dbscan(int minSamples, double minDist, const std::vector<Point> &dataSet, std::vector<label> &labels)
{
    std::vector<std::vector<size_t>> adj;
    std::set<size_t> cores;
    std::vector<bool> visited(dataSet.size(), false);
    labels.resize(dataSet.size());

    for (int i = 0; i < dataSet.size(); i++)
    {
        std::vector<size_t> neighbours;
        for (int j = 0; j < dataSet.size(); j++)
        {
            if (i != j)
            {
                double dist = (dataSet[j] - dataSet[i]).norm();
                if (dist <= minDist)
                {
                    neighbours.emplace_back(j);
                }
            }
        }
        if (neighbours.size() + 1 >= minSamples)
        {
            labels[i].type = Type::Core;
            cores.insert(i);
        }
        else if (!neighbours.empty())
        {
            labels[i].type = Type::Border;
        }
        adj.push_back(std::move(neighbours));
    }

    int cluster = 1;
    
    while (!cores.empty())
    {
        std::queue<size_t> openList;
        size_t startCore = *cores.begin();
        labels[startCore].cluster = cluster;
        visited[startCore] = true;

        openList.push(startCore);

        while (!openList.empty())
        {
            
            auto currNode = openList.front();
            openList.pop();
            auto &neighbours = adj[currNode];
            cores.erase(currNode);

            for (auto neighbour : neighbours)
            {
                if (!visited[neighbour])
                {
                    if (labels[neighbour].type == Type::Core)
                    {
                        openList.push(neighbour);
                    }
                    labels[neighbour].cluster = cluster;
                    visited[neighbour] = true;
                }
            }
        }
        cluster++;
    }
    std::cout << cluster - 1 << "\n";
}

std::vector<Point> loadDataFromCSV(const std::string &filename)
{
    std::vector<Point> data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Error al abrir el archivo CSV: " << filename << std::endl;
        return data;
    }

    // Leer cada línea del archivo CSV
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string xStr, yStr;

        double x, y;
        ss >> x;
        char sep;
        ss >> sep;
        ss >> y;
        data.emplace_back(x, y);
    }

    file.close();
    return data;
}

sf::Color hslToRgb(float h, float s, float l)
{
    auto hue2rgb = [](float p, float q, float t)
    {
        if (t < 0)
            t += 1;
        if (t > 1)
            t -= 1;
        if (t < 1.0f / 6)
            return p + (q - p) * 6 * t;
        if (t < 1.0f / 2)
            return q;
        if (t < 2.0f / 3)
            return p + (q - p) * (2.0f / 3 - t) * 6;
        return p;
    };
    float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
    float p = 2 * l - q;
    float r = hue2rgb(p, q, h + 1.0f / 3);
    float g = hue2rgb(p, q, h);
    float b = hue2rgb(p, q, h - 1.0f / 3);
    return sf::Color(static_cast<sf::Uint8>(r * 255), static_cast<sf::Uint8>(g * 255), static_cast<sf::Uint8>(b * 255));
}

sf::Color getColor(int cluster)
{
    if (cluster == 0)
        return sf::Color::White; // Color para los puntos de ruido (cluster 0)

    float hue = fmod(cluster * 0.27f, 1.0f); // Salto de matiz más amplio para una separación notable
    float saturation = 0.6f + fmod(cluster * 0.1f, 0.4f);
    float lightness = 0.5f;
    return hslToRgb(hue, saturation, lightness);
}

int main()
{
    double minDist = 20.0;
    int minSamples = 5;

    // Cargar dataset desde CSV
    auto dataSet = loadDataFromCSV("dataset.csv");
    if (dataSet.empty())
    {
        std::cerr << "No se pudieron cargar los datos del archivo CSV." << std::endl;
        return -1;
    }
    std::cout << dataSet.size() << "\n";
    std::vector<label> labels;
    dbscan(minSamples, minDist, dataSet, labels);

    sf::RenderWindow window(sf::VideoMode(800, 600), "DBSCAN Clustering Visualization");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        for (size_t i = 0; i < dataSet.size(); i++)
        {
            sf::CircleShape pointShape(5);
            pointShape.setPosition(dataSet[i][0], dataSet[i][1]);
            pointShape.setFillColor(getColor(labels[i].cluster));
            window.draw(pointShape);
        }
        window.display();
    }
    return 0;
}
