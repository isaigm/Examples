#include <iostream>
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
    uint8_t firstByte = dwParam1 & 0xFF;
    if (firstByte == 0x90)
    {
        dwParam1 &= 0xFF00FFFF;
        dwParam1 |= (0x7F0000);
    }
    midiOutShortMsg(proxy, dwParam1);
}
int main()
{
    HMIDIIN instrument = 0;
    //id between [0, midiOutGetNumDevs() - 1]
                            //<- sometimes this number changes
    if (midiOutOpen(&proxy, 2, NULL, 0, NULL) == MMSYSERR_NOERROR)
    {
        std::cout << "ok\n";//use loop midi to create a virtual midi device, in the DAW we going to use this virtual 
                            //midi instead of the physical one, which is not working properly (notes are quiet in random moments)
    }
    std::cout << "number of midi devices: " << midiInGetNumDevs() << "\n"; //id between [0, midiInGetNumDevs() - 1]
                                //<- sometimes this number changes
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
