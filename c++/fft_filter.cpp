#include <iostream>
#include <fftw3.h>
#include <SFML/Audio.hpp>
#define REAL 0
#define IMAG 1
#define WINDOW_SIZE 44100
#define M_PI 3.14159f
/*
    1. Compute FFT in a window of size WINDOW_SIZE
    2. Set bins above the cut off freq to 0
    3. Then compute IFFT
    4. Write to wav file
    5. Loop
    this is a easy way to implement a low pass filter, but we have no control over the frequency response,
    so in general, this method is a bad idea
*/
int main()
{
    fftw_complex* window = new fftw_complex[WINDOW_SIZE];
    fftw_complex* output = new fftw_complex[WINDOW_SIZE];
    fftw_complex* filtered= new fftw_complex[WINDOW_SIZE];
    fftw_plan fft_plan = fftw_plan_dft_1d(WINDOW_SIZE,
        window,
        output,
        FFTW_FORWARD,
        FFTW_ESTIMATE);

    fftw_plan ifft_plan = fftw_plan_dft_1d(WINDOW_SIZE,
        output,
        filtered,
        FFTW_BACKWARD,
        FFTW_ESTIMATE);

    sf::InputSoundFile ifs;
    sf::OutputSoundFile ofs;
    std::string input_file = "test.wav";
    if (ifs.openFromFile(input_file)
    {
        std::string output_file = "filter.wav";
        ofs.openFromFile(output_file, 44100, 1);

        sf::Int16 samples[WINDOW_SIZE * 2];
        while (ifs.read(samples, WINDOW_SIZE * 2) > 0)
        {
            for (int i = 0, j = 0; i < WINDOW_SIZE * 2; i++)
            {
                if (i % 2 == 0)
                {
                    window[j][REAL] = float(samples[i]) / 32767.0f;
                    window[j][IMAG] = 0;
                    j++;
                }
            }
            fftw_execute(fft_plan);
            float fc = 1000;
            int freq_bin = std::floor(float(fc * WINDOW_SIZE) / 44100.0f);
            for (int idx = freq_bin; idx < WINDOW_SIZE; idx++)
            {
                output[idx][REAL] = 0;
                output[idx][IMAG] = 0;
            }
            fftw_execute(ifft_plan);
            sf::Int16 filtered_samples[WINDOW_SIZE];
            for (int i = 0; i < WINDOW_SIZE; i++)
            {
                filtered_samples[i] = filtered[i][REAL];
            }
            ofs.write(filtered_samples, WINDOW_SIZE);
        }
        std::cout << "done\n";

    }

    fftw_destroy_plan(fft_plan);
    fftw_destroy_plan(ifft_plan);
    delete [] window;
    delete [] output;
    delete [] filtered;
    return 0;
}
