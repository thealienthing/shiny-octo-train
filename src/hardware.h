#ifndef HARDWARE_H
#define HARDWARE_H

#define HW_DEBUG_MODE false
#define ENV_PROCESS_SPEED_HZ 100
#define KNOB_COUNT 5
#define ANALOG_SAMPLE_COUNT 3
#define MIDI_TEST_MODE false
#define PATCH_STORAGE_SIZE 2560

#include "daisy_seed.h"
#include "LCDScreen.h"
#include "Menu.h"

using namespace daisy;
using namespace daisy::seed;

class Synth;
class LCDScreen;
class Menu;

//static float DSY_QSPI_BSS flash_buffer[PATCH_STORAGE_SIZE];


class Hardware {
public:
    Hardware (Synth* s);
    ~Hardware () {}
    static Synth* synth;
    //Hardware
    static DaisySeed synth_hw;
    static UartHandler synth_uart;
    static MidiUartHandler synth_midi;
    static UartHandler synth_console;
    static CpuLoadMeter synth_cpu;
    static LCDScreen synth_lcd;
    static I2CHandle i2c;
    static Encoder menu_knob;
    static Menu menu;
    static char _console_out[100];
    static void SerialDebugWriteString(char txBuffer[]);
    void synth_hardware_init();
    static void AudioCallback(AudioHandle::InputBuffer in,
                    AudioHandle::OutputBuffer out,
                    size_t size);
    void ProcessSerial(uint8_t* buff);
    void SynthConfig(Synth* s);

    //Report triggers
    static bool report_amp_env;
private:
    static uint16_t knob_readings[KNOB_COUNT][ANALOG_SAMPLE_COUNT];
    static uint16_t analog_sample_index;
    static int timer5_counter;
    static void Timer5Callback(void* data);
    static void UartCallback(void* data);
    static void HardwareDebugCallback(void* data);
    static void MIDIProcess();
    static void CpuLoadReport();
    static void PushTestMIDI();
    
    static void initialize_flash();

};

#endif