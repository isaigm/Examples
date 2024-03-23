#include <vector>
#include <iostream>
#include <tuple>
#include <map>
#include <cmath>
#include <set>
#include <sstream>
#include <fstream>

using Row = std::vector<double>;
using DataSet = std::vector<Row>;

struct ResultSet
{
    DataSet leftSet;
    DataSet rightSet;
};
struct Split
{
    size_t featureIdx;
    double informationGain{0};
    double threshold;
    ResultSet childs;
};
struct Node
{
    size_t featureIdx;
    double value;
    double threshold;
    double informationGain;
    Node *left = nullptr;
    Node *right = nullptr;
};

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

struct TreeClassifier
{
public:
    TreeClassifier(size_t _classIndex, size_t _maxDepth, size_t _minSampleSplit) : classIndex(_classIndex),
        maxDepth(_maxDepth), minSampleSplit(_minSampleSplit)
    {
    }
    void fit(DataSet &dataSet)
    {
        root = buildTree(dataSet, 0);
    }

    void evaluate(DataSet &dataSet)
    {
        int sucess = 0;
        for (auto &row : dataSet)
        {
            double p = predict(root, row);
            if (p == row[classIndex])
                sucess++;
        }
        std::cout << double(sucess) / double(dataSet.size());
    }
private:
    double predict(Node *node, Row &input)
    {
        if (node == nullptr)
            return -1;
        if (node->left == nullptr && node->right == nullptr)
        {
            return node->value;
        }
        size_t featureIdx = node->featureIdx;
        double threshold = node->threshold;
        if (input[featureIdx] <= threshold)
        {
            return predict(node->left, input);
        }
        return predict(node->right, input);
    }
    Node *buildTree(DataSet &dataSet, size_t currDepth)
    {
        size_t numSamples = dataSet.size();
        Split bestSplit = getBestSplit(dataSet);
        if (numSamples >= minSampleSplit && currDepth <= maxDepth)
        {
            if (bestSplit.informationGain > 0)
            {
                Node *left = buildTree(bestSplit.childs.leftSet, currDepth + 1);
                Node *right = buildTree(bestSplit.childs.rightSet, currDepth + 1);
                Node *parent = new Node();
                parent->featureIdx = bestSplit.featureIdx;
                parent->informationGain = bestSplit.informationGain;
                parent->left = left;
                parent->right = right;
                parent->threshold = bestSplit.threshold;
                return parent;
            }
        }

        Node *node = new Node();
        node->value = getLeafValue(dataSet);
        return node;
    }
    double getLeafValue(DataSet &dataSet)
    {
        int maxCount = INT_MIN;
        double maxClass = INT_MIN;
        std::map<double, int> classes;
        for (auto &row : dataSet)
        {
            classes[row[classIndex]]++;
        }
        for (auto cl : classes)
        {
            if (cl.second > maxCount)
            {
                maxClass = cl.first;
                maxCount = cl.second;
            }
        }
        return maxClass;
    }
    std::set<double> getThresholds(DataSet &dataSet, size_t featureIdx)
    {
        std::set<double> thresholds;
        for (auto &row : dataSet)
        {
            thresholds.insert(row[featureIdx]);
        }
        return thresholds;
    }
    Split getBestSplit(DataSet &dataSet)
    {
        std::vector<size_t> featuresIdx;
        for (size_t i = 0; i < dataSet[0].size(); i++)
        {
            if (i != classIndex)
                featuresIdx.push_back(i);
        }
        Split bestSplit;
        double maxGain = float(INT_MIN);
        for (auto featureIdx : featuresIdx)
        {
            std::set<double> thresholds = std::move(getThresholds(dataSet, featureIdx));
            for (auto threshold : thresholds)
            {
                ResultSet childs = std::move(split(dataSet, featureIdx, threshold));
                double infGain = informationGain(dataSet, childs.leftSet, childs.rightSet);
                if (childs.leftSet.size() > 0 && childs.rightSet.size() > 0)
                {
                    if (infGain > maxGain)
                    {
                        bestSplit.childs = childs;
                        bestSplit.featureIdx = featureIdx;
                        bestSplit.threshold = threshold;
                        bestSplit.informationGain = infGain;
                        maxGain = infGain;
                    }
                }
            }
        }
        return bestSplit;
    }
    double informationGain(DataSet &parentSet, DataSet &leftSet, DataSet &rightSet)
    {
        double leftWeight = double(leftSet.size()) / double(parentSet.size());
        double rightWeight = double(rightSet.size()) / double(parentSet.size());
        return entropy(parentSet) - (leftWeight * entropy(leftSet) + rightWeight * entropy(rightSet));
    }
    ResultSet split(DataSet &dataSet, size_t featureIdx, double threshold)
    {
        DataSet left;
        DataSet right;
        for (auto &row : dataSet)
        {
            if (row[featureIdx] <= threshold)
            {
                left.push_back(row);
            }
            else
            {
                right.push_back(row);
            }
        }
        return {left, right};
    }
    double entropy(DataSet &dataSet)
    {
        std::map<double, int> classes;
        for (auto &row : dataSet)
        {
            classes[row[classIndex]]++;
        }
        double result = 0;
        for (auto &cl : classes)
        {
            double p = double(cl.second) / double(dataSet.size());
            result += p * std::log2(p);
        }
        return -result;
    }
    size_t classIndex;
    size_t maxDepth;
    size_t minSampleSplit;
    Node *root = nullptr;
};
int main()
{
    DataSet dataSet;
    utils::readFromCSV(dataSet, "C:\\Users\\isaig\\Downloads\\iris.csv");
    TreeClassifier treeClassifier(4, 3, 3);
    treeClassifier.fit(dataSet);
    treeClassifier.evaluate(dataSet);
    return 0;
}
