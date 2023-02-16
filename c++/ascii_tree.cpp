#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <ncurses.h>
#include <unistd.h>
#include <utility>
class Timer
{
public:
    void start()
    {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }

    void stop()
    {
        m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }

    double elapsedMilliseconds()
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if (m_bRunning)
        {
            endTime = std::chrono::system_clock::now();
        }
        else
        {
            endTime = m_EndTime;
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
    }

    double elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool m_bRunning = false;
};
void load_tree(std::vector<std::string> &tree)
{
    std::fstream fs("ascii.tree");
    if (fs)
    {
        std::string line;
        while (!fs.eof())
        {
            std::getline(fs, line);
            tree.push_back(line);
        }
        fs.close();
    }
}
void draw_tree(std::vector<std::string> &tree)
{
    for (int y = 0; y < tree.size(); y++)
    {
        for (int x = 0; x < tree[y].size(); x++)
        {
            char ch = tree[y][x];
            switch (ch)
            {
            case '+':
                attron(COLOR_PAIR(1));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(1));
                break;
            case 'w':
                attron(COLOR_PAIR(2));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(2));
                break;
            case 'm':
                attron(COLOR_PAIR(3));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(3));
                break;
            default:
                mvaddch(y, x, ch);
                break;
            }
        }
    }
}
void draw_leds(std::vector<std::string> &tree, std::vector<std::pair<int, int>> &ranges, bool inv)
{
    for (int i = 0; i < ranges.size(); i++)
    {
        for (int r = ranges[i].first; r <= ranges[i].second; r++)
        {
            for (int j = 0; j < tree[r].size(); j++)
            {
                char ch = tree[r][j];
                if (ch == '%')
                {
                    if (i % 2 == 0)
                    {
                        if (inv)
                        {
                            attron(COLOR_PAIR(2));
                            mvaddch(r, j, ch);
                            attroff(COLOR_PAIR(2));
                        }
                        else
                        {
                            attron(COLOR_PAIR(4));
                            mvaddch(r, j, ch);
                            attroff(COLOR_PAIR(4));
                        }
                    }
                    else
                    {
                        if (inv)
                        {
                            attron(COLOR_PAIR(4));
                            mvaddch(r, j, ch);
                            attroff(COLOR_PAIR(4));
                        }
                        else
                        {
                            attron(COLOR_PAIR(2));
                            mvaddch(r, j, ch);
                            attroff(COLOR_PAIR(2));
                        }
                    }
                }
            }
        }
    }
}
int main()
{
    std::vector<std::string> tree;
    std::vector<std::pair<int, int>> ranges{{9, 12}, {15, 18}, {21, 25}, {27, 31}};
    load_tree(tree);
    initscr();
    noecho();
    curs_set(false);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    Timer timer;
    bool inv = false;
    timer.start();
    for (;;)
    {
        draw_tree(tree);
        draw_leds(tree, ranges, inv);
        if (timer.elapsedSeconds() >= 2)
        {
            inv = !inv;
            timer.stop();
            timer.start();
        }
        usleep(16000);
        refresh();
    }
    getch();
    endwin();
    return 0;
}
