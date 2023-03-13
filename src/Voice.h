#ifndef VOICE_H
#define VOICE_H

#include "Oscillator.h"
#include "Envelope.h"
#include "system_consts.h"

class Voice {
public:
    enum Osc_Number {
        Osc1,
        Osc2
    };
    Envelope amp_env;
    // void set_pitch(float osc1_pitch_hz, float osc2_pitch_hz);
    void set_pitch(int note);
    void set_waveform(Osc_Number osc_num, WaveForm waveform);
    void set_osc_volume(Osc_Number osc_num, float amp);
    void set_osc2_offsets(int osc2_smitone, int osc2_tune);
    void init(float sample_rate);
    float get_sample();
    uint8_t note;
private:
    float _sample_rate;
    float _osc1_amp;
    float _osc2_amp;
    Oscillator _osc1;
    Oscillator _osc2;
};

#endif