#include <daisy.h>
#include <daisy_seed.h>
#include "Synth.h"
#include "hardware.h"

using namespace daisy;
using namespace daisy::seed;

//static DaisySeed hw;
extern DaisySeed synth_seed;

static Synth* synth;

void AudioCallback( AudioHandle::InputBuffer in,
                    AudioHandle::OutputBuffer out,
                    size_t size)
{
    //Loop through sample block size and output
    //cpuLoadMeter.OnBlockStart();
    float sample = 0.0;
    for(size_t i = 0; i < size; i++) {
        sample = synth->ProcessAudio();
        out[0][i] = sample;
        out[1][i] = sample;
    }
    //cpuLoadMeter.OnBlockEnd();
}

int main(void)
{
    synth_hardware_init();
    synth = new Synth(synth_hw.AudioSampleRate());
    synth_hw.StartAudio(AudioCallback);
    // Initialize the Daisy Seed hardware
    while(1)
    {
        synth->ProcessHardware();
    }
}
