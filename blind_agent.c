#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#define M_ROWS 41
#define M_COLS 121
static char map[M_ROWS][M_COLS];
enum Dir
{
    WEST,
    EAST,
    SOUTH,
    NORTH
};
struct vector2d
{
    int x;
    int y;
};
int load_map(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
        return 0;
    int i = 0, j = 0;
    char ch = fgetc(fp);
    while (ch != EOF)
    {
        if (ch == '\n')
        {
            i++;
            j = 0;
        }
        else
        {
            map[i][j] = ch;
            j++;
        }
        ch = fgetc(fp);
    }
    return 1;
}

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
void draw_map()
{
    attron(COLOR_PAIR(1));
    for (int i = 0; i < M_ROWS; i++)
    {
        for (int j = 0; j < M_COLS; j++)
        {
            mvaddch(i, j, map[i][j]);
        }
    }
    attroff(COLOR_PAIR(1));
}
enum Dir next_dir(enum Dir curr_dir)
{
    switch (curr_dir)
    {
    case WEST:
        return NORTH;
    case EAST:
        return SOUTH;
    case SOUTH:
        return WEST;
    case NORTH:
        return EAST;
    }
}
int main()
{
    if (load_map("map2.txt"))
    {
        enum Dir curr_dir = EAST;
        struct vector2d pos;
        bool is_colliding = FALSE;
        enum Dir aux;
        enum Dir right;
        char ch;
        pos.x = 1;
        pos.y = 1;
        initscr();
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
        keypad(stdscr, TRUE);
        noecho();
        curs_set(FALSE);
        nodelay(stdscr, TRUE);
        for (;;)
        {
            draw_map();
            if (pos.y < M_ROWS && pos.x < M_COLS)
            {
                attron(COLOR_PAIR(2));
                mvaddch(pos.y, pos.x, '@');
                attroff(COLOR_PAIR(2));
                mvaddch(1, 123, (int) curr_dir + '0');
                refresh();
                right = next_dir(curr_dir);
                switch (right)
                {
                case WEST:
                    ch = map[pos.y][pos.x - 1];
                    break;
                case EAST:
                    ch = map[pos.y][pos.x + 1];
                    break;
                case SOUTH:
                    ch = map[pos.y + 1][pos.x];
                    break;
                case NORTH:
                    ch = map[pos.y - 1][pos.x];
                    break;
                }
                if (ch == ' ')
                {
                    curr_dir = right;
                }
                else if (is_colliding)
                {
                    curr_dir = aux;
                    is_colliding = FALSE;
                }
                switch (curr_dir)
                {
                case WEST:
                    if (map[pos.y][pos.x - 1] == '#')
                    {
                        is_colliding = TRUE;
                        aux = SOUTH;
                    }
                    else
                        pos.x -= 1;
                    break;
                case EAST:
                    if (map[pos.y][pos.x + 1] == '#')
                    {
                        is_colliding = TRUE;
                        aux = NORTH;
                    }
                    else
                        pos.x += 1;
                    break;
                case SOUTH:
                    if (map[pos.y + 1][pos.x] == '#')
                    {
                        is_colliding = TRUE;
                        aux = EAST;
                    }
                    else
                        pos.y += 1;
                    break;
                case NORTH:
                    if (map[pos.y - 1][pos.x] == '#')
                    {
                        is_colliding = TRUE;
                        aux = WEST;
                    }
                    else
                        pos.y -= 1;
                    break;
                }
                clear();
                msleep(65);
            }else{
                mvaddstr(1, 123, "Solved");
                refresh();
                if(getch() == 'q'){
                    break;
                }
            }
        }
        endwin();
    }
    else
        return 1;
    return 0;
}
