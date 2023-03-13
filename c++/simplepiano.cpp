#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

static const int SAMPLE_RATE = 44100;
static const int WIDTH = 400;
static const int HEIGHT = 150;
static const int KEYS = 7;
static const float KEY_WIDTH = float(WIDTH) / KEYS;
static const float KEY_THICKNESS = 1.0f;
enum NOTES {
    DO3 = 261,
    REB3 = 277,
    RE3 = 293,
    MIB3 = 311,
    MI3 = 329,
    FA3 =  349,
    SOLB3 = 369,
    SOL3=  391,
    LAB3 = 415,
    LA3 =  440,
    SIB3= 466,
    SI3 =  493
};
float map(float n, float x1, float x2, float y1, float y2)
{
    float m = (y2 - y1) / (x2 - x1);
    return y1 + m * (n - x1);
}
sf::Int16 squarewave(float t, float freq)
{
    int output = std::floor(2 * freq * t);
    return 32767 * (output & 1 ? -1 : 1);
}
sf::Int16 sawtooth(float t, float freq)
{
    float T = 1.0f / freq;
    float start = T * std::floor(t / T);
    float end = start + T;
    return 32767 * map(t, start, end, -1.0f, 1.0f);
}
class Tone : public sf::Sound
{
public:
    
    Tone(float freq)
    {
        std::vector<sf::Int16> samples;
        float t = 0;
        float dt = 1.0f / SAMPLE_RATE;
        int nsamples = SAMPLE_RATE * (1.0f / freq);
        for (int i = 0; i < nsamples; i++)
        {
            sf::Int16 sample = sawtooth(t, freq);
            t += dt;
            samples.push_back(sample); //left channel
            samples.push_back(sample); //right
        }
        buffer.loadFromSamples(samples.data(), samples.size(), 2, SAMPLE_RATE);
        setBuffer(buffer);
    }
private:
    sf::SoundBuffer buffer;
};
class PianoKey
{
public:
    PianoKey(float x, float y, NOTES note, sf::Keyboard::Key k) :  tone(note) {
        rect.setFillColor(sf::Color::White);
        rect.setSize({ KEY_WIDTH, HEIGHT });
        rect.setPosition(x, y);
        rect.setFillColor(sf::Color::White);
        rect.setOutlineThickness(KEY_THICKNESS); 
        rect.setOutlineColor(sf::Color::Black);
        key = k;
    }
    PianoKey(NOTES note, sf::Keyboard::Key k) : tone(note)
    {
        rect.setFillColor(sf::Color::Black);
        rect.setOutlineThickness(KEY_THICKNESS);
        rect.setOutlineColor(sf::Color::Black);
        isFlatKey = true;
        key = k;
    }
    void render(sf::RenderTarget& rt)
    {
        rt.draw(rect);
    }
  
    void update() {
        if (isPressed())
        {
            if (tone.getStatus() != Tone::Playing)
            {
                tone.play();
                tone.setLoop(true);
                rect.setFillColor(sf::Color::Green);
            }
        }
        else
        {
            tone.setLoop(false);
            if (isFlatKey)
            {
                rect.setFillColor(sf::Color::Black);
            }
            else {
                rect.setFillColor(sf::Color::White);
            }
        }
    }
    sf::Vector2f getPos()
    {
        return rect.getPosition();
    }
    void putAboveOf(PianoKey &other)
    {
        float rightTop = other.getPos().x + KEY_WIDTH;
        rect.setSize({ KEY_WIDTH / 2, HEIGHT / 2 });
        rect.setPosition(rightTop - rect.getSize().x / 2, 0);
    }
private:
    bool isPressed()
    {
        return sf::Keyboard::isKeyPressed(key);
    }
    sf::RectangleShape rect;
    sf::Keyboard::Key key;
    bool isFlatKey = false;
    Tone tone;
};
class Piano
{
public:
    ~Piano()
    {
        for (int i = 0; i < keys.size(); i++)
        {
            delete keys[i];
        }
    }
    Piano() {
        std::vector<NOTES> notes{ DO3, RE3, MI3, FA3, SOL3, LA3, SI3 };
        std::vector<sf::Keyboard::Key> keyboardkeys{sf::Keyboard::Z, sf::Keyboard::X, sf::Keyboard::C, sf::Keyboard::V,
        sf::Keyboard::B, sf::Keyboard::N, sf::Keyboard::M};
        for (int i = 0; i < KEYS; i++)
        {
            PianoKey *key = new PianoKey(i * (KEY_WIDTH + KEY_THICKNESS), 0, notes[i], keyboardkeys[i]);
            keys.push_back(key);
        }
        
        keys.push_back(new PianoKey(REB3,  sf::Keyboard::S));
        keys.push_back(new PianoKey(MIB3,  sf::Keyboard::D));
        keys.push_back(new PianoKey(SOLB3, sf::Keyboard::G));
        keys.push_back(new PianoKey(LAB3,  sf::Keyboard::H));
        keys.push_back(new PianoKey(SIB3,  sf::Keyboard::J));

        keys[7]->putAboveOf(*keys[0]);
        keys[8]->putAboveOf(*keys[1]);
        keys[9]->putAboveOf(*keys[3]);
        keys[10]->putAboveOf(*keys[4]);
        keys[11]->putAboveOf(*keys[5]);
    }
    void update()
    {
        for (auto& key : keys)
        {
            key->update();
        }
    }
    void render(sf::RenderTarget& rt)
    {
        for (auto& key : keys)
        {
            key->render(rt);
        }
    }
private:
    std::vector<PianoKey*> keys;
};
class App {
public:
    App() : window(sf::VideoMode(WIDTH, HEIGHT), "Simple piano")
    {
        window.setVerticalSyncEnabled(true);
    }
    void run()
    {
        while (window.isOpen())
        {
            handleEvents();
            update();
            render();
        }
    }
private:
    void handleEvents()
    {
        sf::Event ev;
        while (window.pollEvent(ev))

        {
            switch (ev.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::GainedFocus:
                hasFocus = true;
                break;
            case sf::Event::LostFocus:
                hasFocus = false;
                break;
            default:
                break;
            }
        }
    }
    void update()
    {
        if (hasFocus)
        {
            piano.update();
        }
     
    }
    void render()
    {
        window.clear(sf::Color::White);
        piano.render(window);
        window.display();
    }
    bool hasFocus = true;
    sf::RenderWindow window;
    Piano piano;
};
int main()
{
    App app;
    app.run();
    return 0;
}
