#include "Synth.h"

Synth::Synth(float sample_rate) {
    MidiUsbHandler::Config midi_cfg;
    midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
    _midi.Init(midi_cfg);
    UartHandler::Config uart_config;
    uart_config.baudrate      = 115200;
    uart_config.periph        = UartHandler::Config::Peripheral::USART_1;
    uart_config.stopbits      = UartHandler::Config::StopBits::BITS_2;
    uart_config.parity        = UartHandler::Config::Parity::NONE;
    uart_config.mode          = UartHandler::Config::Mode::TX_RX;
    uart_config.wordlength    = UartHandler::Config::WordLength::BITS_8;
    uart_config.pin_config.rx = {DSY_GPIOB, 7}; // (USART_1 RX) Daisy pin 15
    uart_config.pin_config.tx = {DSY_GPIOB, 6}; // (USART_1 TX) Daisy pin 14

    _uart.Init(uart_config);

    //cpuLoadMeter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());
    _sample_rate = sample_rate;
    _voice.init(_sample_rate);
    _voice.set_waveform(Voice::Osc_Number::Osc1, WaveForm::Sin);
    _voice.set_waveform(Voice::Osc_Number::Osc2, WaveForm::Saw);
}

float Synth::ProcessAudio() {
    return _voice.get_sample() * _amp;
}

void Synth::ProcessHardware() {
    _midi.Listen();
    float pitch_hz = 0.0;
    char out[100];
    while(_midi.HasEvents())
    {
        /** Pull the oldest one from the list... */
        auto msg = _midi.PopEvent();
        switch(msg.type)
        {
            case NoteOn:
            {
                /** and change the frequency of the oscillator */
                auto note_msg = msg.AsNoteOn();
                if(note_msg.velocity != 0) {
                    pitch_hz = mtof(note_msg.note);
                    _voice.set_pitch(pitch_hz);
                    _amp = (float)note_msg.velocity / 127.0;
                    // sprintf(out, "MIDI %d - HZ %.3f\n", note_msg.note, pitch_hz);
                    sprintf(out, "MIDI %d ON VEL %d\n", note_msg.note, note_msg.velocity);
                    SerialDebugWriteString(out, strlen(out));
                }
                break;
            }
            case NoteOff:
            {
                auto note_msg = msg.AsNoteOff();
                _amp = 0.0;
                sprintf(out, "MIDI %d OFF VEL %d\n", note_msg.note, note_msg.velocity);
                SerialDebugWriteString(out, strlen(out));
                break;
            }
            break;
                // Since we only care about note-on messages in this example
                // we'll ignore all other message types
            default: break;
        }
    }
}



void Synth::SerialDebugWriteString( const char txBuffer[],  int bufferSize){
    _uart.PollTx((uint8_t *)&txBuffer[0],  bufferSize);
    
}

float Synth::mtof(int note) {
    return 440.0 * powf(2.0, (( (float)(note-69) )/12.0));
}