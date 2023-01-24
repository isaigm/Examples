#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
HMIDIOUT proxy = 0;
void CALLBACK MidiInProc(
    HMIDIIN hMidiIn,
    UINT wMsg,
    DWORD_PTR dwInstance,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2)
{
    dwParam1 &= 0xFF00FFFF;
    dwParam1 |= (0x7F0000);
    midiOutShortMsg(proxy, dwParam1);
}
int main()
{
    HMIDIIN instrument = 0;
    if (midiOutOpen(&proxy, 2, NULL, 0, NULL) == MMSYSERR_NOERROR)
    {
        std::cout << "ok\n";
    }
    std::cout << "number of midi devices: " << midiInGetNumDevs() << "\n";

    if (midiInOpen(&instrument, 0, reinterpret_cast<DWORD_PTR>(MidiInProc), 0, CALLBACK_FUNCTION) == MMSYSERR_NOERROR)
    {
        std::cout << "midi device opened\n";
        midiInStart(instrument);
    }
    else
    {
        std::cout << "no midi device found\n";
    }

    while (true)
    {

    }
    return 0;
}
