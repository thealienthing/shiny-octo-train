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

float mtof(int note) {
    return 440.0 * powf(2.0, (( (float)(note-69) )/12.0));
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

    //cpuLoadMeter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());
    sample_rate = hw.AudioSampleRate();
    osc.init(sample_rate);
    hw.StartAudio(AudioCallback);
    osc.set_waveform(WaveForm::Saw);

    char str[100];
    while(1)
    {
        midi.Listen();
        float pitch_hz = 0.0;
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
                        pitch_hz = mtof(note_msg.note);
                        osc.set_pitch(pitch_hz);
                        sprintf(str, "MIDI %d - HZ %.3f\n", note_msg.note, pitch_hz);
                        SerialDebugWriteString(str, strlen(str));
                }
                break;
                    // Since we only care about note-on messages in this example
                    // we'll ignore all other message types
                default: break;
            }
        }
    }
}
