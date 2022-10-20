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
    void init(float sample_rate);
    float get_sample();
private:
    float _pitch;
    float _sample_rate;
    Oscillator _osc1;
    Oscillator _osc2;
};

#endif