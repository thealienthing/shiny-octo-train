#ifndef HARDWARE_H
#define HARDWARE_H

#define TIMER5_SPEED_HZ 50

#include "daisy_seed.h"

using namespace daisy;
using namespace daisy::seed;

class Synth;

class Hardware {
public:
    Hardware () {}
    ~Hardware () {}
    static Synth* synth;
    //Hardware
    static DaisySeed synth_hw;
    static UartHandler synth_uart;
    static MidiUsbHandler synth_midi;
    void synth_hardware_init();
    void SerialDebugWriteString(const char txBuffer[], int bufferSize);
    void SynthConfig(Synth* s);
private:
    static void Timer5Callback(void* data);
    static void MIDIProcess();
};

#endif