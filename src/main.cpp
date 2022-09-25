// GPIO Output
// Example of toggling an LED on/off
//
// Setup:
// * Connect the CATHODE (negative leg) of the LED to GND
// * Connect the ANODE (positive leg, usually longer) of the LED to one side of a resistor (1K)
// * Connect other end of resistor to pin D1 on the daisy.
//
#include "daisy_seed.h"

using namespace daisy;
using namespace daisy::seed;

DaisySeed hw;

int main(void)
{
    // Initialize the Daisy Seed hardware
    hw.Init();

    // Create an LED
    GPIO my_led;
    GPIO my_button;
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

    hw.StartLog(true);
    hw.PrintLine("Program start");


    bool pressed = false;
    // In an infinite loop, we'll continuously turn the LED on/off.
    while(1)
    {
        
        //System::Delay(100);
        temp = enc_val;
        my_encoder.Debounce();
        enc_val += my_encoder.Increment();
        if(temp != enc_val) {
            hw.PrintLine("Encoder value = %d", enc_val);
        }

        if(my_encoder.Pressed() && !pressed) {
            pressed = true;
            hw.PrintLine("Button press!");
        }
        else if(pressed) {
            pressed = false;
            hw.PrintLine("Button released!");
        }


        

        // // Set the pin HIGH
        // my_led.Write(true);
        // // Wait half a second (500ms)
        // System::Delay(500);
        // // Set the pin LOW
        // my_led.Write(false);
        // // Wait another half a second (500ms)
        // System::Delay(500);
        // // You can also use Toggle to change the state
        // my_led.Toggle();
        // // Wait another half a second (500ms)
        // System::Delay(500);
        // // And once more to flip it back
        // my_led.Toggle();
    }
}
