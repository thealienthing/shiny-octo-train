#ifndef HARDWARE_H
#define HARDWARE_H

#define ENV_PROCESS_SPEED_HZ 50
#define KNOB_COUNT 5

#include "daisy_seed.h"
#include "LCDScreen.h"

using namespace daisy;
using namespace daisy::seed;

class Synth;
class LCDScreen;

class Hardware {
public:
    Hardware () {}
    ~Hardware () {}
    static Synth* synth;
    //Hardware
    static DaisySeed synth_hw;
    static UartHandler synth_uart;
    static MidiUartHandler synth_midi;
    static CpuLoadMeter synth_cpu;
    static LCDScreen synth_lcd;
    static I2CHandle i2c;
    static char _console_out[100];
    static void SerialDebugWriteString(char txBuffer[]);
    void synth_hardware_init();
    
    void SynthConfig(Synth* s);

    //Report triggers
    static bool report_amp_env;
private:
    static int knob_readings[KNOB_COUNT];
    static int timer5_counter;
    static void Timer5Callback(void* data);
    static void MIDIProcess();
    static void CpuLoadReport();
};

#endif