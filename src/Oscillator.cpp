#include "Oscillator.h"

void Oscillator::init(float sample_rate) {
    _sample_rate = sample_rate;
    _pitch = 440.0;
    _phase = TWO_PI / _sample_rate;
    _phase_delta = _phase;
}

void Oscillator::set_pitch(float pitch_hz) {
    _pitch = pitch_hz;
}

float Oscillator::get_sample() {
    float sample = sinf(_pitch * _phase);
    _phase += _phase_delta;
    if(_phase > TWO_PI){
        _phase -= TWO_PI;
    }
    return sample;
}