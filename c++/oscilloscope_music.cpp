#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <mutex>
#define WIDTH 800
#define HEIGHT 600
float map(float n, float x1, float x2, float y1, float y2)
{
    float m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}
static const int samplesToStream = 2048;
std::mutex mtx;
class Stream : public sf::SoundStream
{
public:
    void load(std::string path)
    {
        inputFile.openFromFile(path);
        initialize(inputFile.getChannelCount(), inputFile.getSampleRate());
    }
    sf::Int16 *getCurrentSamples() { return samples; }

private:
    virtual bool onGetData(Chunk &data)
    {
        std::lock_guard<std::mutex> lock(mtx);
        int nsamples = inputFile.read(samples, samplesToStream);
        if (nsamples == 0)
            return false;

        data.sampleCount = samplesToStream;
        data.samples = samples;

        return true;
    }
    virtual void onSeek(sf::Time timeOffset)
    {
    }
    sf::InputSoundFile inputFile;
    sf::Int16 samples[samplesToStream];
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "");
    window.setVerticalSyncEnabled(true);
    std::string path = "tes3.wav";
    Stream stream;
    stream.load(path);
    stream.play();
    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }
        std::vector<sf::Vertex> points;
        {
            std::lock_guard<std::mutex> lock(mtx);
            for (int i = 0; i < samplesToStream; i += 2)
            {
                float left = float(stream.getCurrentSamples()[i]) / 32767.0f;
                float right = float(stream.getCurrentSamples()[i + 1]) / 32767.0f;
                float x = map(left, -1, 1, 0, WIDTH);
                float y = map(right, -1, 1, 0, HEIGHT);
                sf::Vertex point;
                point.position = {x, y};
                point.color = sf::Color::Red;
                points.push_back(point);
            }
        }
        
        window.clear();
        window.draw(points.data(), points.size(), sf::Points);
        window.display();
    }
    return 0;
}
