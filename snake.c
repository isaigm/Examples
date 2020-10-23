#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#define WIDTH 800
#define HEIGHT 600
#define SEGMENT_SIZE 20

#ifdef __linux__ 
#include <unistd.h>
void clear_console()
{
    const char* CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}
#elif _WIN32
#include <windows.h>
void clear_console() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(-11);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, 0x0002 | 0x0004 | 0x0001,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}
#else
#endif

enum Dir {
    LEFT,
    RIGHT,
    UP,
    DOWN
};
struct tail_t
{
    enum Dir dir;
    int x, y;
    struct tail_t* next;
};
struct snake_t {
    struct tail_t* last;
    struct tail_t* head;
};
struct tail_t* create_segment(int x_, int y_, enum Dir dir) {
    struct tail_t* sgt = (struct tail_t*)malloc(sizeof(struct tail_t));
    sgt->dir = dir;
    sgt->x = x_;
    sgt->y = y_;
    sgt->next = NULL;
    return sgt;
}
struct snake_t* create_snake(int x, int y, enum Dir dir) {
    struct snake_t* snake = (struct snake_t*)malloc(sizeof(struct snake_t));
    snake->head = create_segment(x, y, dir);
    snake->last = snake->head;
    return snake;
}
void append(struct snake_t* snake, int x, int y) {
    struct tail_t* sgt = create_segment(x, y, snake->last->dir);
    snake->last->next = sgt;
    snake->last = sgt;
}
void spawn_food(int *x, int *y) {
    *x = (rand() % (WIDTH / SEGMENT_SIZE)) * SEGMENT_SIZE;
    *y = (rand() % (HEIGHT / SEGMENT_SIZE)) * SEGMENT_SIZE;
}
void update_pos(struct snake_t* snake, enum Dir curr_dir) {
    int prev_x, prev_y;
    enum Dir prev_dir = curr_dir;
    prev_x = snake->head->x;
    prev_y = snake->head->y;
    switch (curr_dir)
    {
    case LEFT:
        snake->head->x -= SEGMENT_SIZE;
        break;
    case RIGHT:
        snake->head->x += SEGMENT_SIZE;
        break;
    case UP:
        snake->head->y -= SEGMENT_SIZE;
        break;
    case DOWN:
        snake->head->y += SEGMENT_SIZE;
        break;
    default:
        break;
    }
    struct tail_t* ptr = snake->head->next;
    while (ptr != NULL)
    {
        int tx = ptr->x;
        int ty = ptr->y;
        int tdir = ptr->dir;
        ptr->x = prev_x;
        ptr->y = prev_y;
        ptr->dir = prev_dir;
        prev_x = tx;
        prev_y = ty;
        prev_dir = tdir;
        ptr = ptr->next;
    }
}
void draw_snake(SDL_Renderer* renderer, struct snake_t* snake) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    struct tail_t* ptr = snake->head;
    while (ptr != NULL) {
        SDL_Rect rect;
        rect.x = ptr->x;
        rect.y = ptr->y;
        rect.w = SEGMENT_SIZE;
        rect.h = SEGMENT_SIZE;
        ptr = ptr->next;
        SDL_RenderFillRect(renderer, &rect);
    }
}
void find_pos(struct snake_t *snake, int* x, int* y) {
    switch (snake->last->dir)
    {
    case LEFT:
        *x = snake->last->x + SEGMENT_SIZE;
        *y = snake->last->y;
        break;
    case RIGHT:
        *x = snake->last->x - SEGMENT_SIZE;
        *y = snake->last->y;
        break;
    case UP:
        *x = snake->last->x;
        *y = snake->last->y + SEGMENT_SIZE;
        break;
    case DOWN:
        *x = snake->last->x;
        *y = snake->last->y - SEGMENT_SIZE;
        break;
    default:
        break;
    }
}
bool try_collide_with(struct snake_t* snake, int x, int y) {
    return snake->head->x == x && snake->head->y == y;
}
bool check_collisions(struct snake_t *snake) {
    struct tail_t* ptr = snake->head->next;
    while (ptr != NULL) {
        if (try_collide_with(snake, ptr->x, ptr->y)) return true;
        ptr = ptr->next;
    }
    return false;
}
void restart(struct snake_t* snake) {
    struct tail_t *ptr = snake->head->next;
    while (ptr != NULL)
    {
        struct tail_t* aux = ptr;
        ptr = ptr->next;
        free(aux);
        aux = NULL;
    }
    snake->head->x = 60;
    snake->head->y = 20;
    snake->head->dir = RIGHT;
    snake->last = snake->head;
    append(snake, 40, 20);
    append(snake, 20, 20);
}
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("cannot initialize SDL\n");
        exit(1);
    }
    srand(time(NULL));
    int score = 0;
    bool you_lose = false;
    SDL_Window* window = NULL;
    enum Dir curr_dir = RIGHT;
    struct snake_t* snake = create_snake(60, 20, RIGHT);
    bool run = true;
    append(snake, 40, 20, RIGHT);
    append(snake, 20, 20, RIGHT);
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Rect food_rect;
    spawn_food(&food_rect.x, &food_rect.y);
    food_rect.h = SEGMENT_SIZE;
    food_rect.w = SEGMENT_SIZE;
    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false;
                break;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if (curr_dir == UP || curr_dir == DOWN) {
                        curr_dir = LEFT;
                    }
                    break;
                case SDLK_RIGHT:
                    if (curr_dir == UP || curr_dir == DOWN) {
                        curr_dir = RIGHT;
                    }
                    break;
                case SDLK_UP:
                    if (curr_dir == LEFT || curr_dir == RIGHT) {
                        curr_dir = UP;
                    }
                    break;
                case SDLK_DOWN:
                    if (curr_dir == LEFT || curr_dir == RIGHT) {
                        curr_dir = DOWN;
                    }
                    break;
                case SDLK_r:
                    if (you_lose) {
                        restart(snake);
                        spawn_food(&food_rect.x, &food_rect.y);
                        curr_dir = snake->head->dir;
                        score = 0;
                        you_lose = false;
                        clear_console();
                    }
                    break;
                default:
                    break;
                }
            }
        }
        if (!you_lose) {
            update_pos(snake, curr_dir);
            if (check_collisions(snake) || snake->head->x < 0 || snake->head->x >= WIDTH || snake->head->y < 0 || snake->head->y >= HEIGHT) {
                you_lose = true;
                printf("you lose\n");
                printf("score: %d\n", score);
            }
            else if (try_collide_with(snake, food_rect.x, food_rect.y)) {
                enum Dir dir;
                int x, y;
                find_pos(snake, &x, &y);
                append(snake, x, y);
                spawn_food(&food_rect.x, &food_rect.y);
                score++;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        draw_snake(renderer, snake);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &food_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
