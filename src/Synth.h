#include "daisy.h"
#include "daisy_seed.h"
#include "Oscillator.h"

using namespace daisy;
using namespace daisy::seed;


class Synth {
private:
    //Synth component data members
    Oscillator _osc;
    float _amp = 0.5;

    //Audio data
    float _sample_rate = 48000.0;
    
    //Hardware
    DaisySeed _hw;
    UartHandler _uart;
    MidiUsbHandler _midi;


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