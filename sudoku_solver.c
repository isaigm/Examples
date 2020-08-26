#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
bool is_valid(int num, int row, int col, char board[9][9])
{
    int r = row - (row % 3), c = col - (col % 3);
    for (int i = r; i < r + 3; i++)
    {
        for (int j = c; j < c + 3; j++)
        {
            if ((board[i][j] - '0') == num)
            {
                return false;
            }
        }
    }
    for (int i = 0; i < 9; i++)
    {
        int d1 = board[row][i] - '0';
        int d2 = board[i][col] - '0';
        if (d1 == num || d2 == num)
            return false;
    }
    return true;
}
void backtracking(int row, int col, bool *flag, char board[9][9])
{
    while (board[row][col] != '0')
    {
        col++;
        if (col >= 9)
        {
            col = 0;
            row++;
            if (row >= 9)
            {
                *flag = true;
                return;
            }
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (is_valid(i, row, col, board))
        {
            board[row][col] = i + '0';
            int temp_c = col;
            int temp_r = row;
            temp_c++;
            if (temp_c >= 9)
            {
                temp_r++;
                temp_c = 0;
            }
            if (temp_r >= 9)
            {
                *flag = true;
                return;
            }
            backtracking(temp_r, temp_c, flag, board);
            if (*flag)
                return;
            board[row][col] = '0';
        }
    }
    *flag = false;
}
void load(const char *path, char board[9][9])
{
    FILE *fp = fopen(path, "r");
    if (fp != NULL)
    {
        int i = 0, j = 0;
        while (!feof(fp))
        {
            char c = fgetc(fp);
            if (c != '\n' && c != ' ' && i < 9)
            {
                board[i][j] = c;
                j++;
                if (j >= 9)
                {
                    j = 0;
                    i++;
                }
            }
        }

        fclose(fp);
    }
    else
    {
        fprintf(stderr, "file %s not found\n", path);
        exit(1);
    }
}
int main(int argc, char **argv)
{
    char board[9][9];
    char *path = NULL;
    int option_index = 0;
    int c;
    static struct option opt = {"file", required_argument, 0, 0};
    while ((c = getopt_long(argc, argv, "", &opt, &option_index)) != -1)
    {
        if (c == 0)
        {
            load(optarg, board);
        }
        else
        {
            fprintf(stderr, "use ./sudoku --file example.txt\n");
            exit(1);
        }
    }
    bool flag = false;
    clock_t begin = clock();
    backtracking(0, 0, &flag, board);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("time spent in ms: %f\n", time_spent * 1000);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    return 0;
}
