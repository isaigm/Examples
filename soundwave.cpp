#include <iostream>
#include <SFML/Audio.hpp>
#include <math.h>
#define M_PI 3.14159f
#define SAMPLE_RATE 8000
#define Do3 261
#define Re3 293
#define Mi3 329
#define Fa3 349
#define Sol3 391
#define La3 440
#define Si3 493
#define Sib3 466
#define Do4 523
#define Re4 587

float map(float n, float x1, float x2, float y1, float y2)
{
    float m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}

sf::Int16 squarewave(float t, float freq)
{
    float output =  2 * (2 * std::floor(freq * t) - std::floor(2 * freq * t)) + 1;
    return map(output, -1, 1, -32768, 32767);
}
class Tone : public sf::SoundStream
{
public:
    
    Tone(int duration, float freq)
    {
        std::vector<sf::Int16> samples;
        sf::SoundBuffer buffer;
        float t = 0;
        float dt = 1.0f / SAMPLE_RATE;
        int nsamples = SAMPLE_RATE * duration / 1000.0f;
        for (int i = 0; i < nsamples; i++)
        {
            sf::Int16 sample = squarewave(t, freq);
            t += dt;
            samples.push_back(sample);

        }
        buffer.loadFromSamples(samples.data(), samples.size(), 1, SAMPLE_RATE);
       
        // extract the audio samples from the sound buffer to our own container
        m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

        // reset the current playing position 
        m_currentSample = 0;

        // initialize the base class
        initialize(buffer.getChannelCount(), buffer.getSampleRate());
    }

private:

    virtual bool onGetData(Chunk& data)
    {
        
        const int samplesToStream = 1024;

        // set the pointer to the next audio samples to be played
        data.samples = &m_samples[m_currentSample];

        // have we reached the end of the sound?
        if (m_currentSample + samplesToStream <= m_samples.size())
        {
            // end not reached: stream the samples and continue
            data.sampleCount = samplesToStream;
            m_currentSample += samplesToStream;
            return true;
        }
        else
        {
            // end of stream reached: stream the remaining samples and stop playback
            data.sampleCount = m_samples.size() - m_currentSample;
            m_currentSample = m_samples.size();
            return false;
        }
    }

    virtual void onSeek(sf::Time timeOffset)
    {
    }


    std::vector<sf::Int16> m_samples;
    std::size_t m_currentSample;
};

int main()
{
    int notes[] = {Do3, Do3, Do3, Fa3, La3,
                   Do3, Do3, Do3, Fa3, La3,
                   Fa3, Fa3, Mi3, Mi3, Re3, Re3, Do3,
                   Do3, Do3, Do3, Mi3, Sol3,
                   Do3, Do3, Do3, Mi3, Sol3,
                   Do4, Re4, Do4, Sib3, La3, Sol3, Fa3 };
    int size = sizeof(notes) / sizeof(int);
    for (int i = 0; i < size; i++)
    {
        Tone tone(250, notes[i]);

        tone.play();

        while (tone.getStatus() == Tone::Playing)
        {

        }
    }
    return 0;
}

