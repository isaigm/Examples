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
        {
            throw std::runtime_error("file doesnt exist");
        }

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
class LogisticRegressor
{
public:
    LogisticRegressor(size_t inputs) : m_coefficients(inputs + 1), m_inputs(inputs) {}
    const std::vector<T> &getCoefficients()
    {
        return m_coefficients;
    }
    void fit(std::vector<std::vector<T>> &dataSet, T learningRate, size_t batchSize, size_t epochs)
    {
        auto getRandom = [](double min, double max)
        {
            double r = double(rand()) / double(RAND_MAX);
            return r * (max - min) + min;
        };
        for (size_t i = 0; i < m_coefficients.size(); i++)
        {
            m_coefficients[i] = getRandom(-1.0, 1.0);
        }
        std::random_device rd;
        std::mt19937 g(rd());
        for (size_t i = 0; i < epochs; i++)
        {
            size_t init = 0;
            std::shuffle(dataSet.begin(), dataSet.end(), g);
            size_t batchSize_ = batchSize;
            while (batchSize_ > 0 && init < dataSet.size())
            {
                if (init + batchSize_ >= dataSet.size())
                {
                    batchSize_ = (dataSet.size() - init - 1);
                }
                auto start = dataSet.begin() + init;
                auto end = dataSet.begin() + init + batchSize_;
                std::vector<std::vector<T>> result(batchSize_);

                copy(start, end, result.begin());
                sgd(result, learningRate);
                
                init += batchSize_;
            }
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
            T grad{0};
            for (size_t j = 0; j < dataSet.size(); j++)
            {
                auto row = dataSet[j];
                T output = row.back();
                row.pop_back();
                T prediction = predict(row);
                T m = 1.0 / T(dataSet.size());
                if (i == 0)
                {
                    grad += m * (prediction - output);
                }
                else
                {
                    grad += m * (prediction - output) * row[i - 1];
                }
            }
            currCoef -= learningRate * grad;
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
    srand(time(nullptr));
    LogisticRegressor<double> lr(15);
    std::vector<std::vector<double>> dataSet;
    utils::readFromCSV(dataSet, "data_set.csv");
    lr.fit(dataSet, 0.17, 20, 10);
    lr.evaluate(dataSet);
    return 0;
}
