#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>
#include "utils.h"
#include <random>

#define WAVEFORMS_TOTAL 5

enum WaveForm {
    Sin,
    Tri,
    Saw,
    Square,
    WhiteNoise,
    WaveFormEnd
};

class Noise {
private:
    const double _coefficient = 3.5829485 * (pow(0.1, 10.0));
    int32_t _seed = 1;
public:
    Noise() {};
    double process() {
        _seed *= 2385;
        return (_coefficient * _seed) * 1.0;
    }
};

class Oscillator {
public:
    void init(float sample_rate);
    void set_pitch(float pitch_hz);
    void set_waveform(WaveForm waveform);
    float get_sample();
    Noise noise;

    
    
private:
    WaveForm _waveform = WaveForm::Sin;
    float _sample_rate = 0.0;
    float _pitch = 0.0;
    float _pitch_pending = 0.0;
    float _phase = 0.0;
    float _phase_delta = 0.0;
    const float _half_cycle = TWO_PI * 0.5;
    bool _crossing_zero = false;
    bool _changing_pitch = false;
};

#endif