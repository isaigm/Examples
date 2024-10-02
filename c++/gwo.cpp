#include <iostream>
#include <random>
#include <vector>
#include <concepts>
#include <queue>
#include <cassert>
#include <limits>
std::random_device rd;
std::mt19937 mt(rd());
namespace constants
{
    const size_t N = 2; // number of variables
    const int K = 3;    // best k wolves
    const int POP_SIZE = 100;
    const float maxRange = 5;
    const float minRange = -5;
    const float inf = std::numeric_limits<float>::max();
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
    NVector(size_t n = 0) : vec(n, T{}) {}
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
    T fitness()
    {
        return pos[0] * pos[0] - pos[1] * pos[0] + pos[1] * pos[1] + 2 * pos[0] + 4 * pos[1] + 3;
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
auto GWO(int maxIterations)
{
    std::vector<Wolf<T>> population(constants::POP_SIZE);
    std::priority_queue<Wolf<T>, std::vector<Wolf<T>>, Comparator<T>> heap;
    auto addWolf = [&](Wolf<T> &wolf)
    {
        heap.push(wolf);
        if (heap.size() > constants::K)
        {
            heap.pop();
        }
    };
    auto getBestWolves = [&]()
    {
        std::vector<Wolf<T>> bestWolves;
        auto copy = heap;
        while (!copy.empty())
        {
            bestWolves.push_back(copy.top());
            copy.pop();
        }
        return bestWolves;
    };

    for (auto &wolf : population)
    {
        addWolf(wolf);
    }
    auto bestWolves = getBestWolves();
    for (auto i = 0; i < maxIterations; i++)
    {
        T a = 2 * (1 - T(i) / T(maxIterations));
        for (auto &wolf : population)
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
            addWolf(wolf);
        }
        bestWolves = getBestWolves();
    }
    return bestWolves[0];
}
int main()
{
    auto wolf = GWO<float>(200);
    std::cout << wolf.fitness() << "\n";

    return 0;
}
