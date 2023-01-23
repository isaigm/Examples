#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <CL/opencl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#define COLS 1920
#define ROWS 1080

void fill_random(uint8_t* cells)
{
    for (int i = 0; i < ROWS * COLS; i++)
    {
        cells[i] = rand() % 2;
    }
    cells[0] = 1;
}
void load_pattern(uint8_t* cells, const char* file, int start_row, int start_col)
{
    std::ifstream ifs;
    ifs.open(file);
    if (!ifs.is_open())
        exit(1);

    auto fill_row = [](uint8_t* cells, int state, int row, int col, int reps)
    {
        for (int i = col; i < col + reps; i++)
        {
            cells[i + row * COLS] = state;
        }
    };
    int row = start_row;
    int col = start_col;

    auto decode = [&row, &col, &fill_row, start_col, start_row](uint8_t* cells, char ch, int reps)
    {
        switch (ch)
        {
        case 'b':
            fill_row(cells, 0, row, col, reps);
            col += reps;
            break;
        case 'o':
            fill_row(cells, 1, row, col, reps);
            col += reps;
            break;
        case '$':
            col = start_col;
            row += reps;
            break;
        default:
            break;
        }
    };
    while (!ifs.eof())
    {
        std::string line;
        std::getline(ifs, line);
        std::string number;
        for (auto& ch : line)
        {
            if (std::isdigit(ch))
                number.push_back(ch);
            else if (number.size() > 0)
            {
                std::stringstream ss(number);
                int cells_ = 0;
                ss >> cells_;
                decode(cells, ch, cells_);
                number = "";
            }
            else
                decode(cells, ch, 1);
        }
    }
}
int main(int argc, char* argv[])
{
    auto mtp = "__kernel void map_to_pixels(__global unsigned char *cells, __global unsigned char *pixels, int rows, int cols)\n"
        "{\n"
        "\n"
        "    int i = get_global_id(0);\n"
        "    int j = get_global_id(1);\n"
        "    if(i >= rows || j >= cols) return;\n"
        "    int cell = cells[j + i * cols];\n"
        "    if (cell)\n"
        "    {\n"
        "        pixels[(j + i * cols) * 4] = 0;\n"
        "        pixels[(j + i * cols) * 4 + 1] = 0;\n"
        "        pixels[(j + i * cols) * 4 + 2] = 0;\n"
        "        pixels[(j + i * cols) * 4 + 3] = 0;\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        pixels[(j + i * cols) * 4] = 255;\n"
        "        pixels[(j + i * cols) * 4 + 1] = 255;\n"
        "        pixels[(j + i * cols) * 4 + 2] = 255;\n"
        "        pixels[(j + i * cols) * 4 + 3] = 255;\n"
        "    }\n"
        "}";
    auto kernelSource = "__kernel void perform_step(__global unsigned char *cells, __global unsigned char *tempCells, int rows, int cols)\n"
        "{\n"
        "    int row = get_global_id(0);\n"
        "    int col = get_global_id(1);\n"
        "    if(row >= rows || col >= cols) return;\n"
        "    int alive_neighbours = 0;\n"
        "    for(int i = row - 1; i <= row + 1; i++)\n"
        "    {\n"
        "        for(int j = col - 1; j <= col + 1; j++)\n"
        "        {\n"
        "            if(row == i && col == j) continue;\n"
        "            if(i < 0 || j < 0 || j >= cols || i >= rows) continue;\n"
        "            alive_neighbours += cells[j + i * cols];\n"
        "        }\n"
        "    }\n"
        "    if(cells[col + row * cols])\n"
        "    {\n"
        "        if(alive_neighbours == 2 || alive_neighbours == 3)\n"
        "        {\n"
        "            tempCells[col + row * cols] = 1;\n"
        "        }else{\n"
        "            tempCells[col + row * cols] = 0;\n"
        "\n"
        "        }\n"
        "    }else{\n"
        "        tempCells[col + row * cols] = alive_neighbours == 3;\n"
        "    }\n"
        "}";
    srand(time(nullptr));
    sf::RenderWindow window(sf::VideoMode(COLS, ROWS), "GoL");
    window.setVerticalSyncEnabled(true);
    unsigned int rows = ROWS;
    unsigned int cols = COLS;
    size_t ncells = ROWS * COLS * sizeof(uint8_t);
    

    uint8_t* hostCells;
    uint8_t* tempHostCells;
    hostCells = (uint8_t*)malloc(ncells);
    tempHostCells = (uint8_t*)malloc(ncells);

    cl_mem devCells;
    cl_mem devTempCells;
    memset(hostCells, 0, ncells);
    //load_pattern(hostCells, "pattern.rle", 100, 100);
    fill_random(hostCells);
    memcpy(tempHostCells, hostCells, ncells);

    cl_platform_id cpPlatform;    // OpenCL platform
    cl_device_id device_id;       // device ID
    cl_context context;           // context
    cl_command_queue queue;       // command queue
    cl_program program, program2; // program
    cl_kernel kernel;             // kernel
    cl_kernel kernel2;
    size_t* globalSize = (size_t*)malloc(sizeof(size_t) * 2);
    globalSize[0] = ROWS;
    globalSize[1] = COLS;
    cl_int err;

    // Bind to platform
    err = clGetPlatformIDs(1, &cpPlatform, NULL);
    // Get ID for the device
    err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    // Create a context
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    // Create a command queue
    queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);

    program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &err);
   

    std::cout << clBuildProgram(program, 0, NULL, NULL, NULL, NULL) << "\n";
    if (1) {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char* log = (char*)malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        printf("%s\n", log);
    }
    kernel = clCreateKernel(program, "perform_step", &err);
   
    program2 = clCreateProgramWithSource(context, 1, (const char**)&mtp, NULL, &err);
    clBuildProgram(program2, 0, NULL, NULL, NULL, NULL);
    kernel2 = clCreateKernel(program2, "map_to_pixels", &err);
    
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font;
    font.loadFromFile("C:\\Users\\isaig\\Downloads\\fuente.ttf"); // best font
    sf::Text text;
    text.setFont(font);
    texture.create(COLS, ROWS);
    sprite.setTexture(texture);
    sf::Uint8* pixels = new sf::Uint8[ROWS * COLS * 4];

    texture.update(pixels);

    cl_mem devPixels = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(unsigned char) * ROWS * COLS * 4, pixels, NULL);
    devCells = clCreateBuffer(context, CL_MEM_READ_ONLY, ncells, NULL, NULL);
    devTempCells = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, ncells, tempHostCells, NULL);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &devTempCells);
    err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &rows);
    err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &cols);

    clSetKernelArg(kernel2, 1, sizeof(cl_mem), &devPixels);
    clSetKernelArg(kernel2, 2, sizeof(unsigned int), &rows);
    clSetKernelArg(kernel2, 3, sizeof(unsigned int), &cols);

    sf::View view(sf::FloatRect(0, 0, COLS, ROWS));
    sf::Clock clock;
    text.setPosition(10, 10);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Red);
    float t = 0;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        t += dt;
        if (t > 0.4f)
        {
            t = 0;
            char fps[64];
            snprintf(fps, 64, "FPS: %f\n", 1 / dt);
            text.setString(fps);
        }
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();
            if (ev.type == sf::Event::MouseWheelMoved)
            {
                view.zoom(1 - ev.mouseWheel.delta * 0.1);
                window.setView(view);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            view.move(0, -dt * 200);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            view.move(0, dt * 200);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            view.move(-dt * 200, 0);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            view.move(dt * 200, 0);
        }

        clEnqueueWriteBuffer(queue, devCells, CL_TRUE, 0, ncells, hostCells, 0, NULL, NULL);
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &devCells);
        clSetKernelArg(kernel2, 0, sizeof(cl_mem), &devCells);

        clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, NULL, 0, NULL, NULL);
        clEnqueueNDRangeKernel(queue, kernel2, 2, NULL, globalSize, NULL, 0, NULL, NULL);
        clFinish(queue);

        clEnqueueReadBuffer(queue, devTempCells, CL_TRUE, 0, ncells, tempHostCells, 0, NULL, NULL);
        clEnqueueReadBuffer(queue, devPixels, CL_TRUE, 0, sizeof(unsigned char) * ROWS * COLS * 4, pixels, 0, NULL, NULL);
        memcpy(hostCells, tempHostCells, ncells);

        // map_to_pixels(hostCells, pixels);
        texture.update(pixels);
        window.clear();
        window.setView(view);
        window.draw(sprite);
        window.setView(window.getDefaultView());
        window.draw(text);
        window.display();
    }
    clReleaseMemObject(devCells);
    clReleaseMemObject(devTempCells);
    clReleaseMemObject(devPixels);
    clReleaseProgram(program2);
    clReleaseKernel(kernel2);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(hostCells);
    free(tempHostCells);
    delete[] pixels;

    return 0;
}
