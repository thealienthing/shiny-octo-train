#ifndef SYNTH_H
#define SYNTH_H

#include "daisy.h"
#include "daisy_seed.h"
#include "Voice.h"
#include "Oscillator.h"

using namespace daisy;
using namespace daisy::seed;

extern MidiUsbHandler synth_midi;
extern UartHandler synth_uart;
extern DaisySeed synth_seed;


class Synth {
private:
    //Synth component data members
    Voice _voice;
    float _amp = 0.5;

    //Audio data
    float _sample_rate = 48000.0;
    
    void AudioCallback( AudioHandle::InputBuffer in,
                        AudioHandle::OutputBuffer out,
                        size_t size);
    float mtof(int note);
public:
    Synth(float sample_rate);
    void ProcessHardware();
    float ProcessAudio();
    void SerialDebugWriteString(const char txBuffer[],
        int bufferSize);

};

#endif