#include <stdio.h>
#include <CL/cl.hpp>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <chrono>
std::string loadKernelFromFile(std::string path)
{
    std::fstream file(path);
    if (file.is_open())
    {
        std::string content(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        return content;
    }
    return "";
}
int main()
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.size() == 0)
    {
        std::cout << "No OpenCL platform found\n";
        return 1;
    }

    cl::Platform default_platform = platforms[0];
    std::cout << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

    std::vector<cl::Device> devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

    if (devices.size() == 0)
    {
        std::cout << "No devices found\n";
        return 1;
    }
    cl::Device device = devices[0];

    std::cout << device.getInfo<CL_DEVICE_NAME>() << "\n";
    cl::Context context({device});
    cl::CommandQueue queue(context, device);

    cl::Program::Sources sources;

    std::string kernelCode = loadKernelFromFile("C:\\Users\\isaig\\invert_pixels.cl");
    sources.push_back({kernelCode.c_str(), kernelCode.length()});

    cl::Program program(context, sources);
    if (program.build({device}) != CL_SUCCESS)
    {
        std::cout << "error:" << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
    }
    sf::Image image;
    image.loadFromFile("C:\\Users\\isaig\\Downloads\\test11.jpg");
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int sizeOfImage = image.getSize().x * image.getSize().y * 4 * sizeof(uint8_t);
    cl::Buffer bufferPixels(context, CL_MEM_READ_WRITE, sizeOfImage);
    queue.enqueueWriteBuffer(bufferPixels, CL_TRUE, 0, sizeOfImage, image.getPixelsPtr());
    uint8_t *outputPixels = new uint8_t[image.getSize().x * image.getSize().y * 4];
    cl::Kernel kernel(program, "invert_pixels");
    int width = image.getSize().x;
    kernel.setArg(0, bufferPixels);
    kernel.setArg(1, width);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(image.getSize().y));
    queue.finish();
    queue.enqueueReadBuffer(bufferPixels, CL_TRUE, 0, sizeOfImage, outputPixels);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    sf::Image output;
    output.create(image.getSize().x, image.getSize().y, outputPixels);
    output.saveToFile("test011.jpg");
    delete[] outputPixels;
    return 0;
}
