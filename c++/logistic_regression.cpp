#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>

namespace utils
{
    void readFromCSV(std::vector<std::vector<double>> &dataSet, std::string path)
    {
        auto parseLine = [](std::string &line, char sep) -> std::vector<double>
        {
            auto parseNum = [](std::string &str)
            {
                std::stringstream ss(str);
                double val;
                ss >> val;

                return val;
            };
            std::string curr = "";
            std::vector<double> values;
            for (size_t i = 0; i < line.size(); i++)
            {
                char ch = line[i];
                if (curr == "NA")
                {
                    return {};
                }
                if (ch == sep)
                {
                    values.push_back(parseNum(curr));
                    curr = "";
                }
                else
                {
                    curr += ch;
                }
            }
            values.push_back(parseNum(curr));
            return values;
        };
        std::string line;
        std::ifstream file(path);
        bool seenFirstLine = false;
        if (!file.is_open())
            return;

        while (std::getline(file, line))
        {
            if (!seenFirstLine)
            {
                seenFirstLine = true;
            }
            else
            {
                auto row = parseLine(line, ',');
                if (row.size() > 0)
                {
                    dataSet.push_back(row);
                }
            }
        }
    }
};

template <class T>
class LinearRegressor
{
public:
    LinearRegressor(size_t inputs) : m_coefficients(inputs + 1), m_inputs(inputs) {}
    const std::vector<T> &getCoefficients()
    {
        return m_coefficients;
    }
    void fit(std::vector<std::vector<T>> &dataSet, T learningRate, size_t batchSize, size_t epochs)
    {
        auto getRandom = [](float min, float max)
        {
            float r = float(rand()) / float(RAND_MAX);
            return r * (max - min) + min;
        };
        for (size_t i = 0; i < m_coefficients.size(); i++)
        {
            m_coefficients[i] = getRandom(-0.25f, 0.25f);
        }
        std::random_device rd;
        std::mt19937 g(rd());
        size_t init = 0;
        for (size_t i = 0; i < epochs; i++)
        {
            if (init + batchSize >= dataSet.size())
            {
                batchSize = (dataSet.size() - init - 1);
            }
            std::shuffle(dataSet.begin(), dataSet.end(), g);
            auto start = dataSet.begin() + init;
            auto end = dataSet.begin() + init + batchSize;
            std::vector<std::vector<T>> result(batchSize);
            
            copy(start, end, result.begin());
            sgd(result, learningRate);

            init += batchSize;
        }
    }
    T predict(std::vector<T> &input)
    {
        T result{0};
        for (size_t i = 0; i < input.size(); i++)
        {
            result += m_coefficients[i + 1] * input[i];
        }
        result += m_coefficients[0]; // bias

        return sigmoid(result);
    }
    void evaluate(std::vector<std::vector<T>> &dataSet)
    {
        int sucess = 0;
        for (size_t i = 0; i < dataSet.size(); i++)
        {
            auto row = dataSet[i];
            T output = row.back();
            row.pop_back();
            T prediction = predict(row);
            prediction = prediction < 0.5 ? 0 : 1;
            if (prediction == output)
                sucess++;
        }
        std::cout << double(sucess) / double(dataSet.size()) << "\n";
    }

private:
    void sgd(std::vector<std::vector<T>> &dataSet, T learningRate)
    {
        for (size_t i = 0; i < m_coefficients.size(); i++)
        {
            T &currCoef = m_coefficients[i];

            for (size_t j = 0; j < dataSet.size(); j++)
            {
                auto row = dataSet[j];
                T output = row.back();
                row.pop_back();
                T prediction = predict(row);
                T lr = learningRate / T(dataSet.size());
                if (i == 0)
                {
                    currCoef -= lr * (prediction - output);
                }
                else
                {
                    currCoef -= lr * (prediction - output) * row[i - 1];
                }
            }
        }
    }
    T sigmoid(T x)
    {
        return 1 / (1 + std::exp(-x));
    }
    std::vector<T> m_coefficients;
    size_t m_inputs;
};
int main()
{
    LinearRegressor<double> lr(15);
    srand(time(nullptr));
    std::vector<std::vector<double>> dataSet;
    utils::readFromCSV(dataSet, "data_set.csv");
    lr.fit(dataSet, 0.15, 150, 70);
    lr.evaluate(dataSet);
    return 0;
}
