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
        uint8_t buff[50];
        hw->synth_console.BlockingReceive(buff, 50, 10);
        if(strlen((char*)buff) > 0) {
            // synth_hw.PrintLine("%s", buff);
            //Process the string
            hw->ProcessSerial(buff);
        }  
    }
}
