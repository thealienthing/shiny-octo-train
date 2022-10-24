#ifndef VOICE_H
#define VOICE_H

#include "Oscillator.h"

class Voice {
public:
    enum Osc_Number {
        Osc1,
        Osc2
    };
    void set_pitch(float pitch_hz);
    void set_waveform(Osc_Number osc_num, WaveForm waveform);
    void set_osc_volume(Osc_Number osc_num, float amp);
    void init(float sample_rate);
    float get_sample();
private:
    float _pitch;
    float _sample_rate;
    float _osc1_amp;
    float _osc2_amp;
    Oscillator _osc1;
    Oscillator _osc2;
};

#endif