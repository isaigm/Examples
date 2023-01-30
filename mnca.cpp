#include <SFML/Graphics.hpp>
#include <vector>
#include <string.h>
#include <iostream>
#include <CL/opencl.h>
#define WIDTH 512
#define HEIGHT 400
std::vector<std::pair<int, int>> coords1;
std::vector<std::pair<int, int>> coords2;
int get_alive_neighbours(std::vector<std::pair<int, int>>& coords, uint8_t* cells, int row, int col)
{
    int alive = 0;
    for (auto &p: coords)
    {
        int i = (p.first + row) % HEIGHT;
        int j = (p.second + col) % WIDTH;
        if (cells[j + i * WIDTH])
        {
            alive++;
        }
    }
    return alive;
}
void step(uint8_t* cells, uint8_t* next_cells)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            float avg1 = get_alive_neighbours(coords1, cells, i, j) / float(coords1.size());
            float avg2 = get_alive_neighbours(coords2, cells, i, j) / float(coords2.size());
            int state = cells[j + i * WIDTH];
            /*
            if (avg1 >= 0.210f && avg1 <= 0.220f)
            {
                state = 1;
            }
            if (avg1 >= 0.350f && avg1 <= 0.500f)
            {
                state = 0;
            }
            if (avg1 >= 0.750f && avg1 <= 0.850f)
            {
                state = 0;
            }
            if (avg2 >= 0.100f && avg2 <= 0.280f)
            {
                state = 0;
            }
            if (avg2 >= 0.430f && avg2 <= 0.550f)
            {
                state = 1;
            }
            if (avg1 >= 0.120f && avg1 <= 0.150f)
            {
                state = 0;
            }
            */
            if (avg1 >= 0.185 && avg1 <= 0.200)
            {
                state = 1;
            }
            if (avg1 >= 0.343 && avg1 <= 0.580)
            {
                state = 0;
            }
            if (avg1 >= 0.750 && avg1 <= 0.850)
            {
                state = 0;
            }
            if (avg2 >= 0.150 && avg2 <= 0.280)
            {
                state = 0;
            }
            if (avg2 >= 0.445 && avg2 <= 0.680)
            {
                state = 1;
            }
            if (avg1 >= 0.150 && avg1 <= 0.180)
            {
                state = 0;
            }
            next_cells[j + i * WIDTH] = state;
        }
    }
    memcpy(cells, next_cells, sizeof(uint8_t) * WIDTH * HEIGHT);
}
void fill_random(uint8_t* cells, uint8_t* next_cells)
{
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        int state = rand() % 2;
        cells[i] = state;
        next_cells[i] = state;
    }
}
void convert_to_pixels(uint8_t* cells, sf::Uint8* pixels)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (cells[j + i * WIDTH])
            {
                pixels[(j + i * WIDTH) * 4] = 255;
                pixels[(j + i * WIDTH) * 4 + 1] = 255;
                pixels[(j + i * WIDTH) * 4 + 2] = 255;
                pixels[(j + i * WIDTH) * 4 + 3] = 255;
            }
            else
            {
                pixels[(j + i * WIDTH) * 4] = 0;
                pixels[(j + i * WIDTH) * 4 + 1] = 0;
                pixels[(j + i * WIDTH) * 4 + 2] = 0;
                pixels[(j + i * WIDTH) * 4 + 3] = 255;
            }
        }
    }
}
int main()
{
    srand(time(nullptr));
   
    coords1.push_back({ 0, -7 });
    coords1.push_back({ 0, -6 });
    coords1.push_back({ 0, -5 });
    coords1.push_back({ -1, -7 });
    coords1.push_back({ -1, -6 });
    coords1.push_back({ -1, -5 });
    coords1.push_back({ -2, -7 });
    coords1.push_back({ -2, -6 });
    coords1.push_back({ -2, -5 });
    coords1.push_back({ 1, -5 });
    coords1.push_back({ 1, -6 });
    coords1.push_back({ 1, -7 });
    coords1.push_back({ 2, -7 });
    coords1.push_back({ 2, -6 });
    coords1.push_back({ 2, -5 });
    coords1.push_back({ 3, -6 });
    coords1.push_back({ 3, -4 });
    coords1.push_back({ 3, -5 });
    coords1.push_back({ 4, -6 });
    coords1.push_back({ 4, -5 });
    coords1.push_back({ 4, -4 });
    coords1.push_back({ 4, -3 });
    coords1.push_back({ 5, -5 });
    coords1.push_back({ 5, -4 });
    coords1.push_back({ 5, -3 });
    coords1.push_back({ 5, -2 });
    coords1.push_back({ 5, -1 });
    coords1.push_back({ 5, 0 });
    coords1.push_back({ 5, 1 });
    coords1.push_back({ 5, 2 });
    coords1.push_back({ 5, 3 });
    coords1.push_back({ 5, 4 });
    coords1.push_back({ 5, 5 });
    coords1.push_back({ 6, -4 });
    coords1.push_back({ 6, -3 });
    coords1.push_back({ 6, -2 });
    coords1.push_back({ 6, -1 });
    coords1.push_back({ 6, 0 });
    coords1.push_back({ 6, 1 });
    coords1.push_back({ 6, 2 });
    coords1.push_back({ 6, 3 });
    coords1.push_back({ 6, 4 });
    coords1.push_back({ 7, -2 });
    coords1.push_back({ 7, -1 });
    coords1.push_back({ 7, 0 });
    coords1.push_back({ 7, 1 });
    coords1.push_back({ 7, 2 });
    coords1.push_back({ 4, 3 });
    coords1.push_back({ 4, 4 });
    coords1.push_back({ 4, 5 });
    coords1.push_back({ 4, 6 });
    coords1.push_back({ 3, 6 });
    coords1.push_back({ 3, 5 });
    coords1.push_back({ 3, 4 });
    coords1.push_back({ 2, 5 });
    coords1.push_back({ 2, 6 });
    coords1.push_back({ 2, 7 });
    coords1.push_back({ 1, 5 });
    coords1.push_back({ 1, 6 });
    coords1.push_back({ 1, 7 });
    coords1.push_back({ 0, 7 });
    coords1.push_back({ 0, 6 });
    coords1.push_back({ 0, 5 });
    coords1.push_back({ -1, 5 });
    coords1.push_back({ -2, 5 });
    coords1.push_back({ -2, 6 });
    coords1.push_back({ -1, 6 });
    coords1.push_back({ -2, 7 });
    coords1.push_back({ -1, 7 });
    coords1.push_back({ -3, -6 });
    coords1.push_back({ -3, -5 });
    coords1.push_back({ -3, -4 });
    coords1.push_back({ -4, -6 });
    coords1.push_back({ -4, -5 });
    coords1.push_back({ -4, -4 });
    coords1.push_back({ -4, -3 });
    coords1.push_back({ -5, -5 });
    coords1.push_back({ -5, -4 });
    coords1.push_back({ -5, -3 });
    coords1.push_back({ -5, -2 });
    coords1.push_back({ -5, -1 });
    coords1.push_back({ -5, 0 });
    coords1.push_back({ -5, 1 });
    coords1.push_back({ -5, 2 });
    coords1.push_back({ -5, 3 });
    coords1.push_back({ -5, 4 });
    coords1.push_back({ -5, 5 });
    coords1.push_back({ -4, 5 });
    coords1.push_back({ -3, 5 });
    coords1.push_back({ -4, 6 });
    coords1.push_back({ -3, 6 });
    coords1.push_back({ -3, 4 });
    coords1.push_back({ -4, 4 });
    coords1.push_back({ -4, 3 });
    coords1.push_back({ -6, -4 });
    coords1.push_back({ -6, -3 });
    coords1.push_back({ -6, -2 });
    coords1.push_back({ -6, -1 });
    coords1.push_back({ -6, 0 });
    coords1.push_back({ -6, 1 });
    coords1.push_back({ -6, 2 });
    coords1.push_back({ -6, 3 });
    coords1.push_back({ -6, 4 });
    coords1.push_back({ -7, -2 });
    coords1.push_back({ -7, -1 });
    coords1.push_back({ -7, 0 });
    coords1.push_back({ -7, 1 });
    coords1.push_back({ -7, 2 });

    coords2.push_back({ 0, -1 });
    coords2.push_back({ -1, -1 });
    coords2.push_back({ -1, 0 });
    coords2.push_back({ -1, 1 });
    coords2.push_back({ 0, 1 });
    coords2.push_back({ 1, 1 });
    coords2.push_back({ 1, 0 });
    coords2.push_back({ 1, -1 });
    coords2.push_back({ -2, -2 });
    coords2.push_back({ -1, -2 });
    coords2.push_back({ 0, -2 });
    coords2.push_back({ 1, -2 });
    coords2.push_back({ 2, -2 });
    coords2.push_back({ 2, -1 });
    coords2.push_back({ 2, 0 });
    coords2.push_back({ 2, 1 });
    coords2.push_back({ 2, 2 });
    coords2.push_back({ 1, 2 });
    coords2.push_back({ 0, 2 });
    coords2.push_back({ -1, 2 });
    coords2.push_back({ -2, 2 });
    coords2.push_back({ -2, 1 });
    coords2.push_back({ -2, 0 });
    coords2.push_back({ -2, -1 });
    coords2.push_back({ 3, -1 });
    coords2.push_back({ 3, 0 });
    coords2.push_back({ 3, 1 });
    coords2.push_back({ 1, 3 });
    coords2.push_back({ 0, 3 });
    coords2.push_back({ -1, 3 });
    coords2.push_back({ -1, -3 });
    coords2.push_back({ 0, -3 });
    coords2.push_back({ 1, -3 });
    coords2.push_back({ -3, -1 });
    coords2.push_back({ -3, 0 });
    coords2.push_back({ -3, 1 });

    cl_platform_id cpPlatform;    // OpenCL platform
    cl_device_id device_id;       // device ID
    cl_context context;           // context
    cl_command_queue queue;       // command queue
    cl_int err;

    err = clGetPlatformIDs(1, &cpPlatform, NULL);
    err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
    
    int* hCoords1 = new int[coords1.size() * 2];
    int* hCoords2 = new int[coords2.size() * 2];
    int j = 0;
    for (auto& p : coords1)
    {
        hCoords1[j] = p.first;
        hCoords1[j + 1] = p.second;
        j += 2;
    }
    j = 0;
    for (auto& p : coords2)
    {
        hCoords2[j] = p.first;
        hCoords2[j + 1] = p.second;
        j += 2;
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH * 2, HEIGHT * 2), "MNCA");
  
    sf::Texture texture;
    texture.create(WIDTH, HEIGHT);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.scale(2, 2);
    uint8_t* cells = new uint8_t[WIDTH * HEIGHT];
    uint8_t* next_cells = new uint8_t[WIDTH * HEIGHT];
    sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];
    fill_random(cells, next_cells);
    sf::Clock clock;

    cl_kernel stepKernel;
    cl_program stepProgram;
    char* stepKernelSource = "__kernel void perform_step(__global unsigned char *cells,\n"
        "                           __global unsigned char *tempCells,\n"
        "                           __global int *coords1, __global int *coords2,\n"
        "                           int nCoords1, int nCoords2, int rows, int cols) {\n"
        "  int row = get_global_id(0);\n"
        "  int col = get_global_id(1);\n"
        "  if (row >= rows || col >= cols)\n"
        "    return;\n"
        "\n"
        "  int alive1 = 0;\n"
        "  int j = 0;\n"
        "  for (int i = 0; i < nCoords1; i++) {\n"
        "    int y = (coords1[j] + row) % rows;\n"
        "    int x = (coords1[j + 1] + col) % cols;\n"
        "    j += 2;\n"
        "    if (cells[x + y * cols])\n"
        "      alive1++;\n"
        "    \n"
        "  }\n"
        "\n"
        "  int alive2 = 0;\n"
        "  j = 0;\n"
        "  for (int i = 0; i < nCoords2; i++) {\n"
        "    int y = (coords2[j] + row) % rows;\n"
        "    int x = (coords2[j + 1] + col) % cols;\n"
        "    j += 2;\n"
        "    if (cells[x + y * cols])\n"
        "      alive2++;\n"
        "  }\n"
        "  int state = cells[col + row * cols];\n"
        "  float avg1 = alive1 / (float)(nCoords1);\n"
        "  float avg2 = alive2 / (float)(nCoords2);\n"
        "  if (avg1 >= 0.185 && avg1 <= 0.200) {\n"
        "    state = 1;\n"
        "  }\n"
        "  if (avg1 >= 0.343 && avg1 <= 0.580) {\n"
        "    state = 0;\n"
        "  }\n"
        "  if (avg1 >= 0.750 && avg1 <= 0.850) {\n"
        "    state = 0;\n"
        "  }\n"
        "  if (avg2 >= 0.150 && avg2 <= 0.280) {\n"
        "    state = 0;\n"
        "  }\n"
        "  if (avg2 >= 0.445 && avg2 <= 0.680) {\n"
        "    state = 1;\n"
        "  }\n"
        "  if (avg1 >= 0.150 && avg1 <= 0.180) {\n"
        "    state = 0;\n"
        "  }\n"
        "  tempCells[col + row * cols] = state;\n"
        "}";
    stepProgram = clCreateProgramWithSource(context, 1, (const char**)&stepKernelSource, NULL, &err);
    clBuildProgram(stepProgram, 0, NULL, NULL, NULL, NULL);
    stepKernel = clCreateKernel(stepProgram, "perform_step", &err);
    
    cl_kernel mtpKernel;
    cl_program mtpProgram;

    char *mtpKernelSource = "__kernel void map_to_pixels(__global unsigned char *cells, __global unsigned char *pixels, int rows, int cols)\n"
        "{\n"
        "\n"
        "    int i = get_global_id(0);\n"
        "    int j = get_global_id(1);\n"
        "    if(i >= rows || j >= cols) return;\n"
        "    int cell = cells[j + i * cols];\n"
        "    if (cell)\n"
        "    {\n"
        "        pixels[(j + i * cols) * 4] = 255;\n"
        "        pixels[(j + i * cols) * 4 + 1] = 255;\n"
        "        pixels[(j + i * cols) * 4 + 2] = 255;\n"
        "        pixels[(j + i * cols) * 4 + 3] = 255;\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        pixels[(j + i * cols) * 4] = 0;\n"
        "        pixels[(j + i * cols) * 4 + 1] = 0;\n"
        "        pixels[(j + i * cols) * 4 + 2] = 0;\n"
        "        pixels[(j + i * cols) * 4 + 3] = 255;\n"
        "    }\n"
        "}";
    mtpProgram = clCreateProgramWithSource(context, 1, (const char**)&mtpKernelSource, NULL, &err);
    clBuildProgram(mtpProgram, 0, NULL, NULL, NULL, NULL);
    mtpKernel = clCreateKernel(mtpProgram, "map_to_pixels", &err);

    
    cl_mem devCells;
    cl_mem devNextCells;
    cl_mem devCoords1;
    cl_mem devCoords2;
    size_t* globalSize = (size_t*)malloc(sizeof(size_t) * 2);
    globalSize[0] = HEIGHT;
    globalSize[1] = WIDTH;
    
    size_t nCells = WIDTH * HEIGHT * sizeof(uint8_t);

    cl_mem devPixels = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(unsigned char) * WIDTH * HEIGHT * 4, pixels, NULL);

    devCells = clCreateBuffer(context, CL_MEM_READ_ONLY, nCells, NULL, NULL);
    devNextCells = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, nCells, next_cells, NULL);
    devCoords1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, coords1.size() * 2 * sizeof(int), hCoords1, NULL);
    devCoords2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, coords2.size() * 2 * sizeof(int), hCoords2, NULL);
    int rows = HEIGHT;
    int cols = WIDTH;
    int nCoords1 = coords1.size();
    int nCoords2 = coords2.size();
    err |= clSetKernelArg(stepKernel, 1, sizeof(cl_mem), &devNextCells);
    err |= clSetKernelArg(stepKernel, 2, sizeof(cl_mem), &devCoords1);
    err |= clSetKernelArg(stepKernel, 3, sizeof(cl_mem), &devCoords2);
    err |= clSetKernelArg(stepKernel, 4, sizeof(unsigned int), &nCoords1);
    err |= clSetKernelArg(stepKernel, 5, sizeof(unsigned int), &nCoords2);
    err |= clSetKernelArg(stepKernel, 6, sizeof(unsigned int), &rows);
    err |= clSetKernelArg(stepKernel, 7, sizeof(unsigned int), &cols);


    err |= clSetKernelArg(mtpKernel, 1, sizeof(cl_mem), &devPixels);
    err |= clSetKernelArg(mtpKernel, 2, sizeof(unsigned int), &rows);
    err |= clSetKernelArg(mtpKernel, 3, sizeof(unsigned int), &cols);
    window.setFramerateLimit(144);
    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        clEnqueueWriteBuffer(queue, devCells, CL_TRUE, 0, nCells, cells, 0, NULL, NULL);
        clSetKernelArg(stepKernel, 0, sizeof(cl_mem), &devCells);
        clSetKernelArg(mtpKernel, 0, sizeof(cl_mem), &devCells);

        clEnqueueNDRangeKernel(queue, stepKernel, 2, NULL, globalSize, NULL, 0, NULL, NULL);
        clEnqueueNDRangeKernel(queue, mtpKernel, 2, NULL, globalSize, NULL, 0, NULL, NULL);
        clFinish(queue);


        clEnqueueReadBuffer(queue, devNextCells, CL_TRUE, 0, nCells, next_cells, 0, NULL, NULL);
        clEnqueueReadBuffer(queue, devPixels, CL_TRUE, 0, sizeof(unsigned char) * WIDTH * HEIGHT * 4, pixels, 0, NULL, NULL);
        memcpy(cells, next_cells, nCells);


        float dt = clock.restart().asSeconds();
        char fps[64];
        snprintf(fps, 64, "FPS: %f\n", 1 / dt);
        window.setTitle(fps);
        
        //step(cells, next_cells);
        //convert_to_pixels(cells, pixels);
        texture.update(pixels);
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
