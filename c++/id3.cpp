#include <vector>
#include <iostream>
#include <map>
#include <cmath>
#include <set>
#include <sstream>
#include <fstream>
#include <memory>
using InputType  = std::string;
using TargetType = std::string;
using Row        = std::vector<InputType>;
using DataSet    = std::vector<Row>;
using Subsets    = std::map<InputType, DataSet>;

struct Split
{
    size_t  featureIdx;
    double  informationGain{ 0 };
    Subsets childs;
};
struct Node
{
    size_t     featureIdx{};
    TargetType classPredicted{};
    double     informationGain{};
    std::map<InputType, std::shared_ptr<Node>> childs;
    bool isLeaf = true;
};

namespace utils
{
    template <class T>
    void readFromCSV(std::vector<std::vector<T>>& dataSet, std::string path)
    {
        auto parseLine = [](std::string& line, char sep) -> std::vector<T>
        {
                auto parseType = [](std::string& str)
                {
                       std::stringstream ss(str);
                       T val;
                       ss >> val;
                       return val;
                };
                std::string curr = "";
                std::vector<T> values;
                for (size_t i = 0; i < line.size(); i++)
                {
                    char ch = line[i];
                    if (curr == "NA")
                    {
                        return {};
                    }
                    if (ch == sep)
                    {
                        values.push_back(parseType(curr));
                        curr = "";
                    }
                    else
                    {
                        curr += ch;
                    }
                }
                values.push_back(parseType(curr));
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
    void fit(DataSet& dataSet)
    {
        root = buildTree(dataSet, 0);
    }
    void evaluate(DataSet& dataSet)
    {
        int sucess = 0;
        for (auto& row : dataSet)
        {
            TargetType p = predict(root, row);
            if (p == row[classIndex])
                sucess++;
        }
        std::cout << double(sucess) / double(dataSet.size()) << "\n";
    }
    void printTree()
    {
        printTree(root, "", "");
    }
private:
    void printTree(std::shared_ptr<Node> node, std::string value, std::string prefix)
    {
        static int space = 4;
        auto addSpace = [](std::string &input)
        {
           for (int i = 0; i < space; i++)
           {
               input.push_back(' ');
           }
        };
        std::string level;
        if (node->isLeaf)
        {
            level = prefix  +"|__ " + value + " -> " + node->classPredicted;
        }
        else
        {
            level = prefix  +"|__ " + std::to_string(node->featureIdx) + "," + value;
        }
        std::cout << level << "\n";
        addSpace(prefix);
        for (auto& child : node->childs)
        {
            printTree(child.second, child.first, prefix);
        }
    }
    TargetType predict(std::shared_ptr<Node> node, Row& input)
    {
        if (node->isLeaf)
        {
            return node->classPredicted;
        }
        size_t featureIdx = node->featureIdx;  
        return predict(node->childs[input[node->featureIdx]], input);
    }
    
    std::shared_ptr<Node> buildTree(DataSet& dataSet, size_t currDepth)
    {
        size_t numSamples = dataSet.size();
        Split bestSplit   = getBestSplit(dataSet);
       
        if (numSamples >= minSampleSplit && currDepth <= maxDepth)
        {
            if (bestSplit.informationGain > 0)
            {
                auto parent = std::make_shared<Node>();
                parent->isLeaf = false;
                for (auto& subset : bestSplit.childs)
                {
                    parent->childs[subset.first] = buildTree(subset.second, currDepth + 1);
                }
                parent->featureIdx = bestSplit.featureIdx;
                parent->informationGain = bestSplit.informationGain;
                return parent;
            }
        }
        auto node = std::make_shared<Node>();
        node->isLeaf = true;
        node->classPredicted = getLeafValue(dataSet);
        return node;
    }
    TargetType getLeafValue(DataSet& dataSet)
    {
        int maxCount = INT_MIN;
        TargetType maxClass{};
        std::map<TargetType, int> classes;
        for (auto& row : dataSet)
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
    std::set<InputType> getUniqueFeatureValues(DataSet& dataSet, size_t featureIdx)
    {
        std::set<InputType> values;
        for (auto& row : dataSet)
        {
            values.insert(row[featureIdx]);
        }
        return values;
    }
    Split getBestSplit(DataSet& dataSet)
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
            auto uniqueValues = getUniqueFeatureValues(dataSet, featureIdx);
            Subsets childs    = std::move(split(dataSet, featureIdx, uniqueValues));
            double infGain    = informationGain(dataSet, childs);
            if (infGain > maxGain)
            {
                bestSplit.informationGain = infGain;
                bestSplit.featureIdx = featureIdx;
                bestSplit.childs = childs;
                maxGain = infGain;
            }
        }
        return bestSplit;
    }
    double informationGain(DataSet& parentSet, Subsets &subsets)
    {
        double ig = 0;
        for (auto& subset : subsets)
        {
            ig += entropy(subset.second) * double(subset.second.size()) / double(parentSet.size());
        }
        return entropy(parentSet) - ig;
    }
    Subsets split(DataSet& dataSet, size_t featureIdx, std::set<InputType> &values)
    {
        Subsets result;
        for (auto& row : dataSet)
        {
            for (auto& value : values)
            {
                if (row[featureIdx] == value)
                {
                    result[value].push_back(row);
                    break;
                }
            }
        }
        return result;
    }
    double entropy(DataSet& dataSet)
    {
        std::map<TargetType, int> classes;
        for (auto& row : dataSet)
        {
            classes[row[classIndex]]++;
        }
        double result = 0;
        for (auto& cl : classes)
        {
            double p = double(cl.second) / double(dataSet.size());
            result += p * std::log2(p);
        }
        return -result;
    }
    size_t classIndex;
    size_t maxDepth;
    size_t minSampleSplit;
    std::shared_ptr<Node> root;
};

int main()
{
    auto pathToCsv = "C:\\Users\\isaig\\OneDrive\\Desktop\\archive\\PlayTennis.csv";
    DataSet dataSet;
    utils::readFromCSV(dataSet, pathToCsv);
    TreeClassifier treeClassifier(4, 3, 2);
    treeClassifier.fit(dataSet);
    treeClassifier.evaluate(dataSet);
    treeClassifier.printTree();
    return 0;
}
