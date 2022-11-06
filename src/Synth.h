#ifndef SYNTH_H
#define SYNTH_H

#include "daisy.h"
#include "daisy_seed.h"
#include "Voice.h"
#include "Oscillator.h"
#include "hardware.h"


#define NUM_VOICES 8

using namespace daisy;
using namespace daisy::seed;

class Synth {
private:
    //Synth component data members
    Hardware hw;
    WaveForm _osc1_wf = WaveForm::Sin;
    WaveForm _osc2_wf = WaveForm::Sin;
    Voice _voices[NUM_VOICES];
    NoteOnEvent _voice_map[NUM_VOICES];

    uint8_t _voice_count = 0;
    char _console_str[64];

    float _amp = 0.25;

    //Audio data
    float _sample_rate = 48000.0;
    
    
    void AudioCallback( AudioHandle::InputBuffer in,
                        AudioHandle::OutputBuffer out,
                        size_t size);
    float mtof(int note);
public:
    bool led = false;
    Synth(float sample_rate);
    float ProcessAudio();
    // void SerialDebugWriteString(const char txBuffer[],
    //     int bufferSize);

    /*
    These two midi note on/off commands buffer notes is a 
    less than optimal manner. This could be a place to visit to
    free up some processing power since there can be quite a bit
    of time spent copying.
    */
    void MidiNoteOn(NoteOnEvent event);
    void MidiNoteOff(NoteOffEvent event);
    void MidiCCProcess(ControlChangeEvent event);
    void SetVoiceWaveform(Voice::Osc_Number osc, WaveForm waveform);
    void PrintVoiceMap();
};

#endif