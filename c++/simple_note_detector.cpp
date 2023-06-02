#include <iostream>
#include <fftw3.h>
#include <SFML/Audio.hpp>
#include <fstream>
#include <vector>
#include <deque>
#define REAL 0
#define IMAG 1
#define WINDOW_SIZE 4096
#define M_PI 3.14159f
static const float START_FREQ = 16.35f;
static const float RATIO = 1.0594630943592953f;
static const int OCTAVES = 11;
static const float BPM = 120;
static const int16_t PPQ = 480;
static const float MIN_FREQ = START_FREQ * 3;
static const float MAX_FREQ = 5000;
static const float TIME_PER_TICK = 60 / (BPM * PPQ);
float get_freq(int i)
{
    return START_FREQ * std::pow(RATIO, i);
}
struct midi_note
{
    std::string name = "";
    float freq = 0;
    int idx = 0;
    midi_note(std::string _name, float _freq, int _idx): name(_name), freq(_freq), idx(_idx){}
    midi_note(){}
};
midi_note get_note(std::vector<midi_note>& midi_notes, float freq)
{
    midi_note res;
    float diff = 10000000;
    for (auto& n : midi_notes)
    {
        float df = std::abs(freq - n.freq);
        if (df < diff)
        {
            diff = df;
            res = n;
        }
    }
    return res;
}

class MidiWriter
{
public:
    const uint8_t* data()
    {
        return bytes.data();
    }
    MidiWriter()
    {
        bytes.push_back(0x4d);
        bytes.push_back(0x54);
        bytes.push_back(0x68);
        bytes.push_back(0x64);
        //
        bytes.push_back(0);
        bytes.push_back(0);
        bytes.push_back(0);
        bytes.push_back(0x6);
        //
        bytes.push_back(0);
        bytes.push_back(0);
        //
        bytes.push_back(0);
        bytes.push_back(1);
        //
        bytes.push_back((PPQ >> 8) & 0xFF);
        bytes.push_back(PPQ & 0xFF);
        //
        bytes.push_back(0x4d);
        bytes.push_back(0x54);
        bytes.push_back(0x72);
        bytes.push_back(0x6B);

        start_idx = bytes.size();
        bytes.push_back(0);
        bytes.push_back(0);
        bytes.push_back(0);
        bytes.push_back(0);
    }
    void add_note(int note_idx, float note_duration, float silence_duration)
    {
        int silence_ticks = silence_duration / TIME_PER_TICK;
        int note_ticks = note_duration / TIME_PER_TICK;
        if (silence_ticks == 0) {
            bytes.push_back(0);
            byte_counter++;
        }
        else byte_counter += add_variable_len(silence_ticks);

        bytes.push_back(0x90);
        bytes.push_back(note_idx);
        bytes.push_back(100); //vel
        byte_counter += add_variable_len(note_ticks);
        bytes.push_back(0x80);
        bytes.push_back(note_idx);
        bytes.push_back(100); //vel
        byte_counter += 6;
    }
    int size()
    {
        return bytes.size();
    }
    void end()
    {
        bytes[start_idx] = (byte_counter >> 24) & 0xFF;
        bytes[start_idx + 1] = (byte_counter >> 16) & 0xFF;
        bytes[start_idx + 2] = (byte_counter >> 8) & 0xFF;
        bytes[start_idx + 3] = byte_counter & 0xFF;
    }
private:
    int add_variable_len(uint32_t n)
    {
        std::deque<uint8_t> result;

        while (n > 0)
        {
            uint8_t result_byte = 0;
            for (int i = 0; i < 7; i++)
            {
                uint8_t bit = n & 1;
                result_byte |= (bit << i);
                n >>= 1;
            }
            if (n == 0) result_byte |= 0x80;
            result.push_front(result_byte);
        }
        for (auto& b : result)
        {
            bytes.push_back(b);
        }
        return result.size();
    }
    std::vector<uint8_t> bytes;
    int start_idx;
    uint32_t byte_counter = 0;
};
int main()
{
    std::string names[] = { "C", "C#", "D", "D#", "E", "F","F#", "G", "G#", "A", "A#", "B"};
    std::vector<midi_note> midi_notes;
    MidiWriter midi_writer;
    for (int i = 0, note = 0; i < OCTAVES; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            std::string full_name = names[j] + std::to_string(i);
            float freq = get_freq(note);           
            midi_notes.emplace_back(full_name, freq, note);
            note++;
        }
    }
 
    fftw_complex* window = new fftw_complex[WINDOW_SIZE];
    fftw_complex* output = new fftw_complex[WINDOW_SIZE];
    fftw_plan fft_plan = fftw_plan_dft_1d(WINDOW_SIZE,
        window,
        output,
        FFTW_FORWARD,
        FFTW_ESTIMATE);

    sf::InputSoundFile ifs;
    std::string path_file = "C:\\Users\\isaig\\OneDrive\\Documentos\\Audacity\\smb2.wav";

    if (ifs.openFromFile(path_file))
    {
        sf::Int16 samples[WINDOW_SIZE * 2];
        std::vector<midi_note> all_notes;
        while (ifs.read(samples, WINDOW_SIZE * 2) > 0)
        {
            for (int i = 0, j = 0; i < WINDOW_SIZE * 2; i += 2, j++)
            {
                window[j][REAL] = float(samples[i]) / 32767.0f;
                window[j][IMAG] = 0;
            }
            fftw_execute(fft_plan);
            float peak = 0;
            float freq = 0;
            for (int i = 0; i < WINDOW_SIZE / 2; i++)
            {
                float amplitude = std::sqrt(output[i][REAL] * output[i][REAL] + output[i][IMAG] * output[i][IMAG]);
                if (amplitude > peak)
                {
                    freq = (i * ifs.getSampleRate() / float(WINDOW_SIZE));
                    peak = amplitude;
                }
            }
            if (freq > MIN_FREQ && freq < MAX_FREQ)
            {
                auto note = get_note(midi_notes, freq);
                all_notes.push_back(note);
            }
            else
            {
                all_notes.emplace_back("", 0, -1);
            }
        }
        int k = 1;
        float silence_duration = 0;
        float window_time = WINDOW_SIZE / float(ifs.getSampleRate());
        for (int i = 0; i < all_notes.size() - 1; i++)
        {
            if (all_notes[i].idx == all_notes[i + 1].idx)
            {
                k++;
            }
            else
            {
                if (all_notes[i].idx == -1)
                {
                    silence_duration = window_time * k;
                }
                else
                {
                    float note_duration = window_time * k;
                    note_duration *= 3.5f; //change this
                    silence_duration *= 3.5f;
                    midi_writer.add_note(all_notes[i].idx, note_duration, silence_duration);
                    silence_duration = 0;
                }              
                k = 1;
            }
        }
        midi_writer.end();
        std::ofstream output_file("out.mid", std::ios::out | std::ios::binary);
        output_file.write((char*)midi_writer.data(), midi_writer.size());
        output_file.close();
    }
    fftw_destroy_plan(fft_plan);
    delete [] window;
    delete [] output;
    return 0;
}
