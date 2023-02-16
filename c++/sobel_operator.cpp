#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::Image image;

    image.loadFromFile("C:\\Users\\isaig\\Downloads\\test3.jpg");

    for (int i = 0; i < image.getSize().y; i++)
    {
        for (int j = 0; j < image.getSize().x; j++)
        {
            auto pixel = image.getPixel(j, i);
            int avg = (pixel.r + pixel.g + pixel.b) / 3.0f;
            sf::Color color(avg, avg, avg);
            image.setPixel(j, i, color);
        }
    }
    auto convolution = [](sf::Image &image, int i, int j) -> int
    {
        int kernelx[][3] = {{1, 0, -1},
                            {2, 0, -2},
                            {1, 0, -1}};

        int kernely[][3] = {{1, 2, 1},
                            {0, 0, 0},
                            {-1, -2, -1}};
        int gx = 0;
        int gy = 0;
        for (int y = i - 1, row = 0; y <= i + 1; y++, row++)
        {
            for (int x = j - 1, col = 0; x <= j + 1; x++, col++)
            {
                if (x < 0 || y < 0 || x >= image.getSize().x || y >= image.getSize().y)
                    continue;
                int val = image.getPixel(x, y).r;
                gx += kernelx[row][col] * val;
                gy += kernely[row][col] * val;
            }
        }
        return std::sqrt(gx * gx + gy * gy);
    };
    sf::Image outputImage;
    outputImage.create(image.getSize().x, image.getSize().y);
    for (int i = 0; i < image.getSize().y; i++)
    {
        for (int j = 0; j < image.getSize().x; j++)
        {
            int newcolor = convolution(image, i, j);
            outputImage.setPixel(j, i, sf::Color(newcolor, newcolor, newcolor));
        }
    }
    outputImage.saveToFile("test.png");
    return 0;
}
