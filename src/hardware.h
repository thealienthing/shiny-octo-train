#ifndef HARDWARE_H
#define HARDWARE_H

#define HW_DEBUG_MODE false
#define ENV_PROCESS_SPEED_HZ 50
#define KNOB_COUNT 5
#define ANALOG_SAMPLE_COUNT 8

#include "daisy_seed.h"
#include "LCDScreen.h"
#include "Menu.h"

using namespace daisy;
using namespace daisy::seed;

class Synth;
class LCDScreen;
class Menu;

class Hardware {
public:
    Hardware (Synth* s);
    ~Hardware () {}
    static Synth* synth;
    //Hardware
    static DaisySeed synth_hw;
    static UartHandler synth_uart;
    static MidiUartHandler synth_midi;
    static CpuLoadMeter synth_cpu;
    static LCDScreen synth_lcd;
    static I2CHandle i2c;
    static Encoder menu_knob;
    static Menu menu;
    static char _console_out[100];
    static void SerialDebugWriteString(char txBuffer[]);
    void synth_hardware_init();
    
    void SynthConfig(Synth* s);

    //Report triggers
    static bool report_amp_env;
private:
    static uint16_t knob_readings[KNOB_COUNT][ANALOG_SAMPLE_COUNT];
    static uint16_t analog_sample_index;
    static int timer5_counter;
    static void Timer5Callback(void* data);
    static void HardwareDebugCallback(void* data);
    static void MIDIProcess();
    static void CpuLoadReport();
};

#endif