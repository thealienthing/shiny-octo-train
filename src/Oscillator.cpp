#include "daisy_seed.h"
#include "hardware.h"
#include "Oscillator.h"

using namespace daisy;
using namespace daisy::seed;

extern DaisySeed hw;

void Oscillator::init(float sample_rate)
{
    _sample_rate = sample_rate;
    _pitch = 440.0;
    _phase = TWO_PI / _sample_rate;
    _phase_delta = _phase;
}

void Oscillator::set_pitch(float pitch_hz)
{
    _pitch = pitch_hz;
    _phase = 0;
    _phase_delta = (1 / _sample_rate) * TWO_PI * _pitch;
    //_changing_pitch = true;
    hw.PrintLine("Pitch pending");
}

void Oscillator::set_waveform(WaveForm waveform)
{
    _waveform = waveform;
}

float Oscillator::get_sample()
{
    float sample = 0.0;
    // if(_changing_pitch) {
    //     if(((_phase >= 0.0) && (_phase <= START_OF_CYCLE))) {
    //         _changing_pitch = false;
    //         _pitch = _pitch_pending;
    //         //_phase = 0.0;
    //         hw.PrintLine("Pitch changed");
    //     }
    // }
    switch (_waveform)
    {
        case WaveForm::Sin: {
            sample = sinf(_phase);
            break;
        }
        case WaveForm::Square: {
            sample = (_phase < _half_cycle) ? 1.0f: -1.0f;
            break;
        }
        case WaveForm::Saw: {
            sample = ((_phase * TWO_PI_RECIP * 2.0f) * -1.0f) * -1.0f;
        }
        default:
            break;
    }

    _phase += _phase_delta;
    if (_phase > TWO_PI)
    {
        _phase -= TWO_PI;
    }
    return sample;
}