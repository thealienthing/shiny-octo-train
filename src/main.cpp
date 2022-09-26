// GPIO Output
// Example of toggling an LED on/off
//
// Setup:
// * Connect the CATHODE (negative leg) of the LED to GND
// * Connect the ANODE (positive leg, usually longer) of the LED to one side of a resistor (1K)
// * Connect other end of resistor to pin D1 on the daisy.
//
#include "daisy_seed.h"
#include "Oscillator.h"

using namespace daisy;
using namespace daisy::seed;

DaisySeed hw;
CpuLoadMeter cpuLoadMeter;
float sample_rate;
Oscillator osc;

void AudioCallback( AudioHandle::InputBuffer in,
                    AudioHandle::OutputBuffer out,
                    size_t size)
{
    //Loop through sample block size and output
    cpuLoadMeter.OnBlockStart();
    for(size_t i = 0; i < size; i++) {
        float sample = osc.get_sample();
        out[0][i] = sample;
        out[1][i] = sample;
    }
    cpuLoadMeter.OnBlockEnd();
}

void CPULoadCheck(const CpuLoadMeter& meter) {
    // get the current load (smoothed value and peak values)
    const float avgLoad = meter.GetAvgCpuLoad();
    const float maxLoad = meter.GetMaxCpuLoad();
    const float minLoad = meter.GetMinCpuLoad();
    // print it to the serial connection (as percentages)
    hw.PrintLine("Processing Load %:");
    hw.PrintLine("Max: " FLT_FMT3, FLT_VAR3(maxLoad * 100.0f));
    hw.PrintLine("Avg: " FLT_FMT3, FLT_VAR3(avgLoad * 100.0f));
    hw.PrintLine("Min: " FLT_FMT3, FLT_VAR3(minLoad * 100.0f));
}

int main(void)
{
    // Initialize the Daisy Seed hardware

    hw.Init();

    Encoder my_encoder;
    int32_t enc_val = 0;
    int32_t temp = 0;

    // Initialize it to pin D1 as an OUTPUT
    //my_led.Init(D28, GPIO::Mode::OUTPUT);
    //my_button.Init(D15, GPIO::Mode::INPUT);
    
   
    dsy_gpio_pin a, b, click;
    a     = hw.GetPin(15);
    b     = hw.GetPin(16);
    click = hw.GetPin(17);
    my_encoder.Init(a, b, click);

    hw.StartLog();
    hw.PrintLine("Program start");
    cpuLoadMeter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());
    sample_rate = hw.AudioSampleRate();
    osc.init(sample_rate);
    hw.StartAudio(AudioCallback);
    
    

    
    while(1)
    {
        // don't spam the serial connection too much
        //CPULoadCheck(cpuLoadMeter);
        //hw.PrintLine("Sample rate = " FLT_FMT3, FLT_VAR3(sample_rate));
        //System::Delay(1000);
        System::Delay(10);
        temp = enc_val;
        my_encoder.Debounce();
        enc_val = abs(enc_val + my_encoder.Increment()) % 5;
        if(temp != enc_val) {
            hw.PrintLine("Pitch set: %d", enc_val);
            switch(enc_val) {
                case 0:
                    osc.set_pitch(261.626);
                    break;
                case 1:
                    osc.set_pitch(293.625);
                    break;
                case 2:
                    osc.set_pitch(329.628);
                    break;
                case 3:
                    osc.set_pitch(349.228);
                    break;
                case 4:
                    osc.set_pitch(391.995);
                    break;
                
            }
        }

    }
}
