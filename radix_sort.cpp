#include <iostream>
#include <vector>
int count_digits(int k)
{
    int c = 0;
    while (k > 0)
    {
        k /= 10;
        c++;
    }
    return c;
}
int main()
{
    std::vector<int> nums{123, 2, 3, 444, 1, 21, 20, 23, 11, 33, 32, 0};
    std::vector<std::vector<int>> ld(10);
    int max_digits = 0;
    for (size_t i = 0; i < nums.size(); i++)
    {
        max_digits = std::max(max_digits, count_digits(nums[i]));
    }
    int d = 1;
    for (int i = 0; i < max_digits; i++)
    {
        while (nums.size() > 0)
        {
            ld[(nums.front() / d) % 10].push_back(nums.front());
            nums.erase(nums.begin());
        }
        d *= 10;
        for (int j = 0; j <= 9; j++)
        {
            while (ld[j].size() > 0)
            {
                nums.push_back(ld[j].front());
                ld[j].erase(ld[j].begin());
            }
            
        }
    }
    for(const auto &it: nums){
        std::cout << it << " ";
    }
    std::cout << std::endl;
    return 0;
}
