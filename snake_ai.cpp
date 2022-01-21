#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <queue>
#include <tuple>
#include <set>
#define WIDTH 800
#define HEIGHT 600
#define SEGMENT_SIZE 20
#define ROWS HEIGHT / SEGMENT_SIZE
#define COLS WIDTH / SEGMENT_SIZE
#define MAX_BUFFER_LEN 1 << 8

uint8_t map[ROWS][COLS];
enum Dir
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};
struct point
{
    int x, y;
    point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};
struct path_node
{
public:
    path_node(int x, int y, std::shared_ptr<path_node> parent = nullptr)
    {
        this->x = x;
        this->y = y;
        this->parent = parent;
    }
    std::vector<std::shared_ptr<path_node>> getNeighbours()
    {
        std::vector<point> coords{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        std::vector<std::shared_ptr<path_node>> neighbours;
        for (auto &c : coords)
        {
            point new_point(x + c.x, y + c.y);
            if (new_point.x >= 0 && new_point.x < COLS && new_point.y >= 0 && new_point.y < ROWS)
            {
                if (map[new_point.y][new_point.x] == 0)

                {
                    neighbours.push_back(std::make_shared<path_node>(new_point.x, new_point.y, std::make_shared<path_node>(x, y, parent)));
                }
            }
        }
        return neighbours;
    }
    int x, y;
    std::shared_ptr<path_node> parent;
};

void spawn_food(int *x, int *y)
{
    *x = (rand() % (WIDTH / SEGMENT_SIZE - 2) + 1) * SEGMENT_SIZE;
    *y = (rand() % (HEIGHT / SEGMENT_SIZE - 2) + 1) * SEGMENT_SIZE;
}
void update_pos(std::vector<point> &snake, Dir curr_dir)
{
    int prev_x, prev_y;
    prev_x = snake[0].x;
    prev_y = snake[0].y;
    switch (curr_dir)
    {
    case LEFT:
        snake[0].x -= SEGMENT_SIZE;
        break;
    case RIGHT:
        snake[0].x += SEGMENT_SIZE;
        break;
    case UP:
        snake[0].y -= SEGMENT_SIZE;
        break;
    case DOWN:
        snake[0].y += SEGMENT_SIZE;
        break;
    default:
        break;
    }
    int i1 = floor(snake[0].y / SEGMENT_SIZE);
    int j1 = floor(snake[0].x / SEGMENT_SIZE);
    map[i1][j1] = 1;
    for (int i = 1; i < snake.size(); i++)
    {
        auto &curr = snake[i];
        int tx = curr.x;
        int ty = curr.y;
        curr.x = prev_x;
        curr.y = prev_y;
        i1 = floor(prev_y / SEGMENT_SIZE);
        j1 = floor(prev_x / SEGMENT_SIZE);
        map[i1][j1] = 1;
        prev_x = tx;
        prev_y = ty;
        if (i == snake.size() - 1)
        {
            i1 = floor(prev_y / SEGMENT_SIZE);
            j1 = floor(prev_x / SEGMENT_SIZE);
            map[i1][j1] = 0;
        }
    }
}
void draw_snake(SDL_Renderer *renderer, std::vector<point> &snake)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (auto &p : snake)
    {
        SDL_Rect rect;
        rect.x = p.x;
        rect.y = p.y;
        rect.w = SEGMENT_SIZE;
        rect.h = SEGMENT_SIZE;
        SDL_RenderFillRect(renderer, &rect);
    }
}
bool try_collide_with(std::vector<point> &snake, int x, int y)
{
    return snake[0].x == x && snake[0].y == y;
}
bool check_collisions(std::vector<point> &snake)
{
    for (int i = 1; i < snake.size(); i++)
    {
        if (try_collide_with(snake, snake[i].x, snake[i].y))
        {
            return true;
        }
    }
    return false;
}
void append(std::vector<point> &snake, int x, int y)
{
    int i = floor(y / SEGMENT_SIZE);
    int j = floor(x / SEGMENT_SIZE);
    snake.push_back(point(x, y));
}
void restart(std::vector<point> &snake)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            map[i][j] = 0;
        }
    }
    snake.erase(snake.begin(), snake.end());
    append(snake, 60, 20);
    append(snake, 40, 20);
    append(snake, 20, 20);
}

SDL_Rect get_text(int x, int y, SDL_Renderer *renderer, TTF_Font *font, SDL_Texture **texture, SDL_Surface **surface, char *format, ...)
{
    char buff[MAX_BUFFER_LEN];
    SDL_Color color = {255, 255, 255};
    va_list args;
    va_start(args, format);
    vsnprintf(buff, sizeof(buff), format, args);
    *surface = TTF_RenderText_Solid(font, buff, color);
    *texture = SDL_CreateTextureFromSurface(renderer, *surface);
    int textW = 0;
    int textH = 0;
    SDL_QueryTexture(*texture, NULL, NULL, &textW, &textH);
    SDL_Rect dstrect = {x - textW, y, textW, textH};
    va_end(args);
    return dstrect;
}
void destroy_text(SDL_Texture *texture, SDL_Surface *surface)
{
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
void try_spawn_food(std::vector<point> &snake, int *x, int *y)
{
    bool valid_pos;
    do
    {
        valid_pos = true;
        spawn_food(x, y);
        for (auto &p : snake)
        {
            if (p.x == *x && p.y == *y)
            {
                valid_pos = false;
                break;
            }
        }
    } while (!valid_pos);
}
point bfs(std::vector<point> &snake, int goal_x, int goal_y)
{
    int start_x = floor(snake[0].x / SEGMENT_SIZE);
    int start_y = floor(snake[0].y / SEGMENT_SIZE);

    std::queue<std::shared_ptr<path_node>> q;
    std::set<std::tuple<int, int>> visited;
    q.push(std::make_shared<path_node>(start_x, start_y));
    std::shared_ptr<path_node> n(nullptr);
    while (!q.empty())
    {
        n = q.front();
        q.pop();
        if (n->x == goal_x && n->y == goal_y)
        {
            break;
        }
        visited.insert(std::make_tuple(n->x, n->y));
        auto neighbours = n->getNeighbours();
        for (auto &child : neighbours)
        {
            if (visited.find(std::make_tuple(child->x, child->y)) == visited.end())
            {
                q.push(child);
                visited.insert(std::make_tuple(child->x, child->y));
            }
        }
    }
    std::vector<point> path;
    while (n.get() != nullptr)
    {
        path.push_back(point(n->x, n->y));
        n = n->parent;
    }

    return path[path.size() - 2];
}
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("cannot initialize SDL\n");
        exit(1);
    }
    TTF_Init();
    srand(time(NULL));
    int score = 0;
    bool you_lose = false;
    SDL_Window *window = NULL;
    Dir curr_dir = RIGHT;
    std::vector<point> snake;
    bool run = true;
    restart(snake);

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    TTF_Font *font = TTF_OpenFont("Minecraft.ttf", 25);
    if (font == NULL)
    {
        printf("cannot found the font\n");
        exit(1);
    }
    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    SDL_Surface *surface2 = NULL;
    SDL_Texture *texture2 = NULL;
    SDL_Rect food_rect;
    SDL_Rect dstrect = get_text(WIDTH - 100, 0, renderer, font, &texture, &surface, "Score: 0");
    SDL_Rect msg = get_text(WIDTH - 200, HEIGHT / 2, renderer, font, &texture2, &surface2, "You lose :( press R for play again :)");
    try_spawn_food(snake, &food_rect.x, &food_rect.y);
    food_rect.h = SEGMENT_SIZE;
    food_rect.w = SEGMENT_SIZE;
    while (run)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                run = false;
                break;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r && you_lose)
            {

                restart(snake);
                try_spawn_food(snake, &food_rect.x, &food_rect.y);
                curr_dir = RIGHT;
                score = 0;
                you_lose = false;
            }
        }
        if (!you_lose)
        {

            if (check_collisions(snake) || snake[0].x < 0 || snake[0].x >= WIDTH || snake[0].y < 0 || snake[0].y >= HEIGHT)
            {
                you_lose = true;
                printf("you lose\n");
                printf("score: %d\n", score);
                destroy_text(texture, surface);
                dstrect = get_text(WIDTH - 100, 0, renderer, font, &texture, &surface, "Score: 0");
            }
            else if (try_collide_with(snake, food_rect.x, food_rect.y))
            {
                append(snake, snake.back().x, snake.back().y);
                try_spawn_food(snake, &food_rect.x, &food_rect.y);
                score++;
                destroy_text(texture, surface);
                dstrect = get_text(WIDTH - 100, 0, renderer, font, &texture, &surface, "Score: %d", score);
            }

            int goalx = floor(food_rect.x / SEGMENT_SIZE);
            int goaly = floor(food_rect.y / SEGMENT_SIZE);
            auto point = bfs(snake, goalx, goaly);
            point.x *= 20;
            point.y *= 20;
            if (point.x == snake[0].x)
            {
                if (point.y >= snake[0].y)
                {
                    curr_dir = DOWN;
                }
                else
                    curr_dir = UP;
            }
            else if (point.y == snake[0].y)
            {
                if (point.x >= snake[0].x)
                {
                    curr_dir = RIGHT;
                }
                else
                    curr_dir = LEFT;
            }
            update_pos(snake, curr_dir);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        if (!you_lose)
        {
            draw_snake(renderer, snake);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &food_rect);
        }
        else
        {
            SDL_RenderCopy(renderer, texture2, NULL, &msg);
        }
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
    }
    destroy_text(texture, surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
