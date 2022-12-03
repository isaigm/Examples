#include <iostream>
#include <vector>
// Y = a + b * x                     x     y
float pardva(std::vector<std::pair<float, float>> &dataset, float a, float b)
{
    float s = dataset.size() * a;
    for (int i = 0; i < dataset.size(); i++)
    {
        s += (b * dataset[i].first - dataset[i].second);
    }
    return s;
}
float pardvb(std::vector<std::pair<float, float>> &dataset, float a, float b)
{
    float s = 0;
    for (int i = 0; i < dataset.size(); i++)
    {
        s += (a + b * dataset[i].first - dataset[i].second) * dataset[i].first;
    }
    return s;
}
void gradient_descent(std::vector<std::pair<float, float>> &dataset)
{
    float a = 10, b = 10;
    float learning_rate = 0.00002f;
    while (true)
    {
     
        a += -pardva(dataset, a, b) * learning_rate;
        b += -pardvb(dataset, a, b) * learning_rate;
        printf("%f %f\n", a, b);
    }
}
int main()
{
    std::vector<std::pair<float, float>> dataset;
    float c = 10.0;
    for(int i = 0; i < 100; i++)
    {
        dataset.push_back({c, c * 1.8f + 32.0f});
        c += 0.1f;
    }
    gradient_descent(dataset);
    return 0;
}