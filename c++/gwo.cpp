#include <iostream>
#include <random>
#include <vector>
#include <concepts>
#include <queue>
#include <cassert>
#include <SFML/Graphics.hpp>

std::random_device rd;
std::mt19937 mt(rd());
const int WIDTH = 1280;
const int HEIGHT = 720;
const int WOLF_RADIUS = 5;
namespace constants
{
    const size_t N = 2; // number of variables
    const int K = 3;    // best k wolves
    const int POP_SIZE = 100;
    const float maxRange = 10.0f;
    const float minRange = -10.0f;
}
template <std::floating_point T>
T random(T min, T max)
{
    std::uniform_real_distribution<T> distribution(min, max);
    return distribution(mt);
}
template <std::floating_point T>
struct NVector
{
    NVector(size_t n) : vec(n, T{}) {}
    NVector() {}
    T operator[](size_t i)
    {
        return vec[i];
    }
    NVector operator*(const NVector &other)
    {
        assert(other.vec.size() == vec.size());
        NVector result(other.vec.size());
        for (auto i = 0; i < vec.size(); i++)
        {
            result.vec[i] = vec[i] * other.vec[i];
        }
        return result;
    }

    void operator+=(const NVector &other)
    {
        assert(other.vec.size() == vec.size());
        for (auto i = 0; i < vec.size(); i++)
        {
            vec[i] += other.vec[i];
        }
    }
    NVector operator+(const NVector &other)
    {
        assert(other.vec.size() == vec.size());
        NVector result(other.vec.size());
        for (auto i = 0; i < vec.size(); i++)
        {
            result.vec[i] = vec[i] + other.vec[i];
        }
        return result;
    }
    NVector operator-(const NVector &other)
    {
        assert(other.vec.size() == vec.size());
        NVector result(other.vec.size());
        for (auto i = 0; i < vec.size(); i++)
        {
            result.vec[i] = vec[i] - other.vec[i];
        }
        return result;
    }
    NVector &operator*(T alpha)
    {
        for (auto i = 0; i < vec.size(); i++)
        {
            vec[i] *= alpha;
        }
        return *this;
    }
    void abs()
    {
        for (auto i = 0; i < vec.size(); i++)
        {
            vec[i] = std::abs(vec[i]);
        }
    }
    void clamp(T min, T max)
    {
        for (size_t i = 0; i < vec.size(); i++)
        {
            if (vec[i] < min)
                vec[i] = min;
            if (vec[i] > max)
                vec[i] = max;
        }
    }
    std::vector<T> vec;
};

template <std::floating_point T>
struct Wolf
{
    Wolf() : pos(constants::N)
    {
        for (auto i = 0; i < constants::N; i++)
        {
            pos.vec[i] = random(constants::minRange, constants::maxRange);
        }
    }

    T fitness() // Levy Function N. 13
    {
        T x = pos[0];
        T y = pos[1];
        T term1 = std::sin(3 * M_PI * x) * std::sin(3 * M_PI * x);
        T term2 = (x - 1) * (x - 1) * (1 + std::sin(3 * M_PI * y) * std::sin(3 * M_PI * y));
        T term3 = (y - 1) * (y - 1) * (1 + std::sin(2 * M_PI * y) * std::sin(2 * M_PI * y));
        return term1 + term2 + term3;
    }

    NVector<T> pos;
};

template <std::floating_point T>
class Comparator
{
public:
    bool operator()(Wolf<T> &w1, Wolf<T> &w2)
    {
        return w1.fitness() < w2.fitness();
    }
};
template <std::floating_point T>
struct GWOState
{
    GWOState() : population(constants::POP_SIZE) {}
    std::vector<Wolf<T>> population;
    std::priority_queue<Wolf<T>, std::vector<Wolf<T>>, Comparator<T>> heap;
    void addWolf(Wolf<T> &wolf)
    {
        heap.push(wolf);
        if (heap.size() > constants::K)
        {
            heap.pop();
        }
    }
    auto getBestKWolves()
    {
        std::vector<Wolf<T>> bestWolves;
        auto copy = heap;
        while (!copy.empty())
        {
            bestWolves.push_back(copy.top());
            copy.pop();
        }
        return bestWolves;
    }
};
template <std::floating_point T>
T map(T n, T x1, T x2, T y1, T y2)
{
    T m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}
sf::Color interpolateColor(const sf::Color &start, const sf::Color &end, float factor)
{
    sf::Color result;
    result.r = static_cast<sf::Uint8>(start.r + factor * (end.r - start.r));
    result.g = static_cast<sf::Uint8>(start.g + factor * (end.g - start.g));
    result.b = static_cast<sf::Uint8>(start.b + factor * (end.b - start.b));
    return result;
}

template <std::floating_point T>
void renderState(sf::RenderTarget &rt, GWOState<T> &state)
{
    T minFitness = std::numeric_limits<T>::max();
    T maxFitness = std::numeric_limits<T>::lowest();
    for (auto &wolf : state.population)
    {
        T fitness = wolf.fitness();
        if (fitness < minFitness)
            minFitness = fitness;
        if (fitness > maxFitness)
            maxFitness = fitness;
    }
    for (auto &wolf : state.population)
    {
        sf::CircleShape point(WOLF_RADIUS);
        point.setOrigin({WOLF_RADIUS, WOLF_RADIUS});
        T fitness = wolf.fitness();
        float normalizedFitness = (fitness - minFitness) / (maxFitness - minFitness); 
        sf::Color startColor = sf::Color::Red; 
        sf::Color endColor = sf::Color::Blue;  
        sf::Color pointColor = interpolateColor(startColor, endColor, normalizedFitness);
        point.setFillColor(pointColor);
        sf::Vector2<T> pos;
        auto x = wolf.pos[0];
        auto y = wolf.pos[1];
        pos.x = map(x, constants::minRange, constants::maxRange, T(0), T(WIDTH));
        pos.y = map(y, constants::minRange, constants::maxRange, T(HEIGHT), T(0));
        point.setPosition(pos);
        rt.draw(point);
    }
}
template <std::floating_point T>
void GWO(int currIteration, int maxIterations, GWOState<T> &state)
{
    if (currIteration >= maxIterations)
        return;
    auto bestWolves = state.getBestKWolves();
    std::cout << bestWolves[0].pos[0] << "," << bestWolves[0].pos[1] << "\n";
    T a = 2 * (1 - T(currIteration) / T(maxIterations));
    for (auto &wolf : state.population)
    {
        NVector<T> nextPos(constants::N);
        for (auto i = 0; i < constants::K; i++)
        {
            T A = 2 * a * random(0.0, 1.0) - a;
            T C = 2 * random(0.0, 1.0);
            auto &bestWolf = bestWolves[i];
            auto D = bestWolf.pos * C - wolf.pos;
            D.abs();
            nextPos += bestWolf.pos - D * A;
        }
        nextPos = nextPos * (1.0 / T(constants::K));
        wolf.pos = nextPos;
        wolf.pos.clamp(constants::minRange, constants::maxRange);
        state.addWolf(wolf);
    }
}
int main()
{
    GWOState<float> currState;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "");
    window.setVerticalSyncEnabled(true);
    float time = 0;
    int currIteration = 0;
    int maxIterations = 200;
    for (auto &wolf : currState.population)
    {
        currState.addWolf(wolf);
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
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter)
            {
                GWO<float>(currIteration, maxIterations, currState);
                currIteration++;
            }
        }
        window.clear();
        renderState<float>(window, currState);
        window.display();
    }
    return 0;
}
