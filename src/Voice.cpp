#include "daisy_seed.h"
#include "Voice.h"

using namespace daisy;
using namespace daisy::seed;

extern DaisySeed hw;

void Voice::init(float sample_rate)
{
    _sample_rate = sample_rate;
    _osc1.init(_sample_rate);
    _osc2.init(_sample_rate);
}

void Voice::set_pitch(float pitch_hz)
{
    _pitch = pitch_hz;
    _osc1.set_pitch(pitch_hz);
    _osc2.set_pitch(pitch_hz);
}

void Voice::set_waveform(Osc_Number osc_num, WaveForm waveform)
{
    if(osc_num == Osc_Number::Osc1) {
        _osc1.set_waveform(waveform);
    }
    else if(osc_num == Osc_Number::Osc2) {
        _osc2.set_waveform(waveform);
    }
    else {
        //Log invalid Oscillator enum passed
    }
    //hw.PrintLine("Waveform: %d", (int)waveform);
}

float Voice::get_sample()
{
    float sample1 = _osc1.get_sample();
    float sample2 = _osc2.get_sample() * 0.1;
    return sample1 + sample2;
}