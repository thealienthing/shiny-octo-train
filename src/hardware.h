#ifndef HARDWARE_H
#define HARDWARE_H

#define ENV_PROCESS_SPEED_HZ 50

#include "daisy_seed.h"
#include "i2c-lcd.h"

using namespace daisy;
using namespace daisy::seed;

class Synth;

class Hardware {
public:
    Hardware () {}
    ~Hardware () {}
    static Synth* synth;
    //Hardware
    static DaisySeed synth_hw;
    static UartHandler synth_uart;
    static MidiUsbHandler synth_midi;
    static CpuLoadMeter synth_cpu;
    static I2CHandle i2c;
    static char _console_out[100];
    static void SerialDebugWriteString(char txBuffer[]);
    void synth_hardware_init();
    
    void SynthConfig(Synth* s);

    //Report triggers
    static bool report_amp_env;
    void LCD_SetScreen(char* str);
private:
    static int timer5_counter;
    
    static void Timer5Callback(void* data);
    static void MIDIProcess();
    static void CpuLoadReport();
};

#endif