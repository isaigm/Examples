#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include <stdio.h>
#include <SDL.h>
#include <iostream>
#include <SFML/Audio.hpp>

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif
int delay = 0;
float scale = 1;
float alpha = 0;
float map(float n, float x1, float x2, float y1, float y2)
{
    float m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}
float clamp(float x)
{
    if(x < -1) return -1;
    if(x > 1) return 1;
    return x;
}
float convertToFloat(sf::Int16 x)
{
    return map(x, INT16_MIN, INT16_MAX, -1.0f, 1.0f);
}
sf::Int16 convertTo16Bit(float x)
{
    return map(x, -1.0f, 1.0f, INT16_MIN, INT16_MAX);
}
class MyStream : public sf::SoundStream
{
public:
    void load(const sf::SoundBuffer &buffer)
    {
        // extract the audio samples from the sound buffer to our own container
        m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

        // reset the current playing position
        m_currentSample = 0;

        // initialize the base class
        initialize(buffer.getChannelCount(), buffer.getSampleRate());
    }

private:
    virtual bool onGetData(Chunk &data)
    {

        const int samplesToStream = 2048;

        std::vector<sf::Int16> left_channel;
        std::vector<sf::Int16> right_channel;
        for (int i = m_currentSample; i < m_currentSample + samplesToStream; i++)
        {
            if (i % 2 == 0)
            {
                left_channel.push_back(m_samples[i]);
            }
            else
            {
                right_channel.push_back(m_samples[i]);
            }
        }
        int j = m_currentSample;
      
        for (int i = 0; i < left_channel.size(); i++)
        {
            if (i - delay >= 0)
            {
                float out = convertToFloat(left_channel[i]) + convertToFloat(left_channel[i - delay]) * alpha;
                left_channel[i] = convertTo16Bit(clamp(out));

                out = convertToFloat(right_channel[i]) + convertToFloat(right_channel[i - delay]) * alpha;
                right_channel[i] = convertTo16Bit(clamp(out));
            }
            m_samples[j] = left_channel[i] * scale;
            m_samples[j + 1] = right_channel[i] * scale;
            j += 2;
        }
        data.samples = &m_samples[m_currentSample];
        if (m_currentSample + samplesToStream <= m_samples.size())
        {
            data.sampleCount = samplesToStream;
            m_currentSample += samplesToStream;
            return true;
        }
        else
        {
            data.sampleCount = m_samples.size() - m_currentSample;
            m_currentSample = m_samples.size();
            return false;
        }
    }

    virtual void onSeek(sf::Time timeOffset)
    {
        m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
    }

    std::vector<sf::Int16> m_samples;
    std::size_t m_currentSample;
};

// Main code
int main(int, char **)
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to the latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("ImGui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 300, window_flags);

    // Setup SDL_Renderer instance
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    // Main loop
    bool done = false;

    sf::SoundBuffer buffer;
    buffer.loadFromFile("dynatron.wav");

    // initialize and play our custom stream
    MyStream stream;
    stream.load(buffer);
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("COMB FILTER");
            ImGui::Text("Filtro comb usando SFML");
            ImGui::SliderFloat("factor de escalamiento", &alpha, 0.0f, 1.0f);
            ImGui::SliderFloat("escalar salida", &scale, 0.0f, 1.0f);
            ImGui::SliderInt("delay (en samples)", &delay, 0, 2048);
        
            if (ImGui::Button("Escuchar"))
            {
                stream.play();
            }
            ImGui::End();
        }

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
