#include <daisy.h>
#include <daisy_seed.h>
#include "hardware.h"
#include "Synth.h"


using namespace daisy;
using namespace daisy::seed;

//static DaisySeed hw;
//extern DaisySeed synth_seed;

Synth* synth;
Hardware* hw;

int main(void)
{
    hw = new Hardware(synth);
    hw->synth_hardware_init();
    // hw->SynthConfig(synth);
    
    // Initialize the Daisy Seed hardware
    while(1)
    {
        // hw.synth_hw.DelayMs(500);
        // hw.LCD_SetScreen("Hello");
        // hw.synth_hw.PrintLine("Log Hello");
        // hw.synth_hw.DelayMs(500);
        // hw.LCD_SetScreen("Goodbye");
        // hw.synth_hw.PrintLine("Log Goodbye");
    }
}
