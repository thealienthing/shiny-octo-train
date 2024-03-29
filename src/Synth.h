#ifndef SYNTH_H
#define SYNTH_H

#include "daisy.h"
#include "daisy_seed.h"
#include "Voice.h"
#include "Envelope.h"
#include "Filter.h"
#include "Menu.h"
#include "Oscillator.h"
#include "PatchParams.h"
#include "hardware.h"
#include "system_consts.h"

using namespace daisy;
using namespace daisy::seed;

class Synth {
private:
    //Synth component data members
    WaveForm _osc1_wf = WaveForm::Sin;
    WaveForm _osc2_wf = WaveForm::Sin;
    Voice _voices[NUM_VOICES];
    NoteOnEvent _voice_map[NUM_VOICES];

    uint8_t _voice_count = 0;

    //Audio data
    float _sample_rate = 48000.0;
    
    
    void AudioCallback( AudioHandle::InputBuffer in,
                        AudioHandle::OutputBuffer out,
                        size_t size);
public:
    bool led = false;
    double _amp = 0.05;
    Synth(float sample_rate);
    Filter* filter;
    PatchParams patch_params;

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
    void SetOscillatorLevel(Voice::Osc_Number osc, float level);
    void SetOscillator2Pitch(int osc2_semitone, int osc2_tune);
    void SetFilterType(FilterType filter_type);
    void SetFilterCutoff(uint32_t freq_hz);
    void SetFilterResonance(float resonance);
    void PrintVoiceMap();
    void PrintVoiceInfo(uint8_t voice);
    
    void SetEnvelopeAttack(uint16_t attack);
    void SetEnvelopeDecay(uint16_t decay);
    void SetEnvelopeSustain(float sustain);
    void SetEnvelopeRelease(uint16_t release);

    void AmpEnvelopeProcess();
    void ApplyPatch();

    //Buffer that is read by hardware in the timer5 interupt
    char _console_str[100];
    bool _console_print = false; //when this is true, timer interupt
                                 //will print _console_str and set
                                 //_console_print to false
};

#endif