#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
//maybe not the best implementation
void merge(int l1, int r1, int l2, int r2, std::vector<int> &v)
{
    std::vector<int> sorted;
    int temp = l1;
    while (l1 < r1 + 1 && l2 < r2 + 1)
    {
        if (v[l1] < v[l2])
        {
            sorted.push_back(v[l1]);
            l1++;
        }
        else
        {
            sorted.push_back(v[l2]);
            l2++;
        }
    }
    if (l1 == r1 + 1)
    {
        for (int i = l2; i <= r2; i++)
        {
            sorted.push_back(v[i]);
        }
    }
    else if (l2 == r2 + 1)
    {
        for (int i = l1; i <= r1; i++)
        {
            sorted.push_back(v[i]);
        }
    }
    for (int i = temp, j = 0; i <= r2; i++, j++)
    {
        v[i] = sorted[j];
    }
}
void merge_sort(int left, int right, std::vector<int> &v)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        merge_sort(left, mid, v);
        merge_sort(mid + 1, right, v);
        merge(left, mid, mid + 1, right, v);
    }
}
int main()
{
    srand(time(nullptr));
    std::vector<int> vec;
    for (int i = 0; i < 201; i++)
    {
        int sign = rand() % 2 == 1 ? 1 : -1;
        vec.push_back(sign * (rand() % 1000));
        std::cout << vec[i] << "\n";
    }
    std::cout << "-------\n";
    merge_sort(0, vec.size() - 1, vec);
    for (const auto &x : vec)
    {
        std::cout << x << "\n";
    }
    return 0;
}
