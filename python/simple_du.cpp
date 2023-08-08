#include <iostream>
#include <filesystem>
#include <queue>
#define K 32

enum class Mode
{
    DIRECTORIES,
    FILES
};
struct path
{
    std::string full_name;
    float mb_size = 0;
};

class comparator
{
public:
    bool operator()(path &p1, path &p2)
    {
        return p1.mb_size > p2.mb_size;
    }
};

using heap = std::priority_queue<path, std::vector<path>, comparator>;
void add_entry(heap& sizes, std::string name, float size)
{
    path d;
    d.full_name = std::move(name);
    d.mb_size = size;
    sizes.push(d);
    if (sizes.size() > K)
    {
        sizes.pop();
    }
}
float dir_size(std::filesystem::path curr_dir, heap &sizes)
{
    float size = 0;
    try {
        for (auto const& dir_entry : std::filesystem::directory_iterator{ curr_dir })
        {
            if (dir_entry.is_directory())
            {
                size += dir_size(dir_entry, sizes);
            }
            else size += float(dir_entry.file_size()) / (1024.0f * 1024.0f);
        }
        add_entry(sizes, curr_dir.string(), size);
       
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << "\n";
    }
   
    return size;
}
void file_size(std::filesystem::path curr_dir, heap& sizes)
{
    try {
        for (auto const& dir_entry : std::filesystem::directory_iterator{ curr_dir })
        {
            if (dir_entry.is_directory())
            {
                file_size(dir_entry, sizes);
            }
            else
            {
                float size = float(dir_entry.file_size()) / (1024.0f * 1024.0f);
                add_entry(sizes, std::filesystem::absolute(dir_entry).string(), size);

            }
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}

void top(std::string path_name, Mode mode)
{
    std::filesystem::path path{ path_name };
    heap sizes;
  
    if (mode == Mode::DIRECTORIES)
    {
        dir_size(path, sizes);
    }
    else if (mode == Mode::FILES)
    {
        file_size(path, sizes);
    }
    while (sizes.size() > 0)
    {
        auto curr_dir = sizes.top();
        std::cout << curr_dir.full_name << ": " << curr_dir.mb_size << "mb\n";
        sizes.pop();
    }
}
int main()
{
    //test
    std::string path_name = "C:\\Users\\isaig";
    top(path_name, Mode::FILES);
    return 0;
}
