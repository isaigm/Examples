#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
int main(int argc, char **argv){

    if(argc < 2 ){
        std::cerr << "expected path to a wav file\n";
        return 1;
    }
    std::string path(argv[1]);
    sf::SoundBuffer sb;
    if(!sb.loadFromFile(path))
    {
        return 1;
    }
    std::vector<sf::Int16> leftChannel;
    std::vector<sf::Int16> rightChannel;
    auto samples = sb.getSamples();
    auto size = sb.getSampleCount();
    if(sb.getChannelCount() == 1)
    {
        std::vector<sf::Int16> channel;
        for(int i = 0; i < size; i++)
        {
            channel.push_back(samples[i]);
        }
        std::reverse(channel.begin(), channel.end());
        sb.loadFromSamples(channel.data(), size, sb.getChannelCount(), sb.getSampleRate());
        sb.saveToFile("reverse" + path);
        return 0;
    }
    for(int i = 0; i < size; i += 2)
    {
        leftChannel.push_back(samples[i]);
        rightChannel.push_back(samples[i + 1]);
    }
    std::reverse(leftChannel.begin(), leftChannel.end());
    std::reverse(rightChannel.begin(), rightChannel.end());
    sf::Int16 *newSamples = new sf::Int16[size];
    for(int i = 0, k = 0; i < size; i += 2, k++)
    {
        newSamples[i] = leftChannel[k];
        newSamples[i + 1] = rightChannel[k];
    }
    sb.loadFromSamples(newSamples, size, sb.getChannelCount(), sb.getSampleRate());
    sb.saveToFile("reverse" + path);
    delete [] newSamples;
    return 0;
}
