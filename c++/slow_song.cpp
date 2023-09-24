#include <SFML/Audio.hpp>
#include <iostream>
static const int N = 512;
int main(int argc, char **argv)
{
    std::string path(argv[1]);
    sf::InputSoundFile ifs;
    if(!ifs.openFromFile(path)) throw std::runtime_error("file doesnt exist");
    sf::OutputSoundFile ofs;
    float delay = 300; //seconds
    float duration = ifs.getDuration().asSeconds();
    int totalSamples = ifs.getSampleCount();
    float sr = totalSamples / (duration + delay);
    
   
    ofs.openFromFile(path + ".wav", int(sr), ifs.getChannelCount());
 
    while(true)
    {
        sf::Int16 samples[N];
        int n = ifs.read(samples, N);
        if(n < N) break;
        sf::Int16 slowedSamples[2 * N];
        for(int i = 0, j = 0; i < N; i += 2, j += 4)
        {
            auto left            = samples[i];
            auto right           = samples[i + 1];
            slowedSamples[j]     = left;
            slowedSamples[j + 1] = right;
            slowedSamples[j + 2] = left;
            slowedSamples[j + 3] = right;
        }
        ofs.write(slowedSamples, 2 * N);
    }
    return 0;
}
