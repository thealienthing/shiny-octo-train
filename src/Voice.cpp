#include "daisy_seed.h"
#include "Voice.h"

using namespace daisy;
using namespace daisy::seed;

extern DaisySeed hw;

void Voice::init(float sample_rate)
{
    note = 0;
    _sample_rate = sample_rate;
    _osc1_amp = 0.5;
    _osc1.init(_sample_rate);
    _osc2_amp = 0.5;
    _osc2.init(_sample_rate);
    amp_env.reset();
}

void Voice::set_pitch(int note)
{
    _osc1.set_pitch(mtof(note));
    _osc2.set_pitch(mtof(note+_osc2.semitone_offset));
}

void Voice::set_osc2_offsets(int osc2_semitone, int osc2_tune) {
    _osc2.semitone_offset = osc2_semitone;
    _osc2.tune_offset = osc2_tune;
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
    if(amp_env.phase != Envelope::Phase::TAIL_OFF) {
        float sample1 = (_osc1.get_sample() * _osc1_amp);
        float sample2 = (_osc2.get_sample() * _osc2_amp);
        return ((sample1 + sample2)/2 * amp_env.val)*0.5;
    }
    else {
        return 0.0;
    }
}

void Voice::set_osc_volume(Osc_Number osc_num, float amp) {
    if(osc_num == Osc1)
        _osc1_amp = amp;
    else if(osc_num == Osc2)
        _osc2_amp = amp;
    else
        return;
}