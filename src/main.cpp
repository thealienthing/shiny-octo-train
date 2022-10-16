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
#include "hardware.h"

using namespace daisy;
using namespace daisy::seed;

extern DaisySeed hw;

CpuLoadMeter cpuLoadMeter;
float sample_rate;
MidiUsbHandler midi;
static UartHandler uart;
Oscillator osc;



void AudioCallback( AudioHandle::InputBuffer in,
                    AudioHandle::OutputBuffer out,
                    size_t size)
{
    //Loop through sample block size and output
    cpuLoadMeter.OnBlockStart();
    float sample = 0.0;
    for(size_t i = 0; i < size; i++) {
        sample = osc.get_sample() * 0.1;
        out[0][i] = sample;
        out[1][i] = sample;
        // out[0][i] = 0.0f;
        // out[1][i] = 0.0f;
    }
    cpuLoadMeter.OnBlockEnd();
}

void CPULoadCheck(const CpuLoadMeter& meter) {
    // get the current load (smoothed value and peak values)
    const float avgLoad = meter.GetAvgCpuLoad();
    const float maxLoad = meter.GetMaxCpuLoad();
    const float minLoad = meter.GetMinCpuLoad();
    // print it to the serial connection (as percentages)
    // hw.PrintLine("Processing Load %:");
    // hw.PrintLine("Max: " FLT_FMT3, FLT_VAR3(maxLoad * 100.0f));
    // hw.PrintLine("Avg: " FLT_FMT3, FLT_VAR3(avgLoad * 100.0f));
    // hw.PrintLine("Min: " FLT_FMT3, FLT_VAR3(minLoad * 100.0f));
}

static void SerialDebugWriteString( const char txBuffer[],  int bufferSize){
    uart.PollTx((uint8_t *)&txBuffer[0],  bufferSize);
    
}

int main(void)
{
    // Initialize the Daisy Seed hardware
    hw.Configure();
    hw.Init();
    MidiUsbHandler::Config midi_cfg;
    midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
    midi.Init(midi_cfg);
    UartHandler::Config uart_config;
    uart_config.baudrate      = 115200;
    uart_config.periph        = UartHandler::Config::Peripheral::USART_1;
    uart_config.stopbits      = UartHandler::Config::StopBits::BITS_2;
    uart_config.parity        = UartHandler::Config::Parity::NONE;
    uart_config.mode          = UartHandler::Config::Mode::TX_RX;
    uart_config.wordlength    = UartHandler::Config::WordLength::BITS_8;
    uart_config.pin_config.rx = {DSY_GPIOB, 7}; // (USART_1 RX) Daisy pin 15
    uart_config.pin_config.tx = {DSY_GPIOB, 6}; // (USART_1 TX) Daisy pin 14

    uart.Init(uart_config);
    
    int pitch = 0;
    int wave = 0;

    
    //cpuLoadMeter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());
    sample_rate = hw.AudioSampleRate();
    osc.init(sample_rate);
    hw.StartAudio(AudioCallback);
    SerialDebugWriteString("HELLO", 5);
    //WaveForm waveform = WaveForm::WhiteNoise;

    char str[100];
    while(1)
    {
        midi.Listen();
        bool pitch_changed = false;
        while(midi.HasEvents())
        {
            /** Pull the oldest one from the list... */
            auto msg = midi.PopEvent();
            switch(msg.type)
            {
                case NoteOn:
                {
                    /** and change the frequency of the oscillator */
                    auto note_msg = msg.AsNoteOn();
                    if(note_msg.velocity != 0)
                        //osc.SetFreq(mtof(note_msg.note));
                        pitch++;
                        pitch = pitch % 5;
                        pitch_changed = true;
                        sprintf(str, "Note %d played at channel %d with vel %d", note_msg.note, note_msg.channel, note_msg.velocity);
                        SerialDebugWriteString(str, strlen(str));
                        //SerialDebugWriteString(msg, 4);
                        //uart.PollTx((uint8_t *)&msg[0],  4);
                        
                }
                break;
                    // Since we only care about note-on messages in this example
                    // we'll ignore all other message types
                default: break;
            }
        }
        
        //hw.PrintLine("Pitch set: %d", pitch);
        if(pitch_changed) {
            pitch_changed = false;
            switch(pitch) {
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
