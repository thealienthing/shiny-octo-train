#include "hardware.h"
#include "Synth.h"


MidiUsbHandler Hardware::synth_midi;
DaisySeed Hardware::synth_hw;
UartHandler Hardware::synth_uart;
Synth* Hardware::synth;

void Hardware::Timer5Callback(void* data)
{
    bool led = (System::GetNow() & 1023) > 511;
    synth_hw.SetLed(led);

    MIDIProcess();
    synth->AmpEnvelopeProcess();
    
}

void Hardware::MIDIProcess() {
    synth_midi.Listen();
    while(synth_midi.HasEvents())
    {
        /** Pull the oldest one from the list... */
        auto msg = synth_midi.PopEvent();
        //auto msg = MidiEvent();
        switch(msg.type)
        {
            case NoteOn:
            {
                synth->MidiNoteOn(msg.AsNoteOn());
                /** and change the frequency of the oscillator */
                // auto note_msg = msg.AsNoteOn();
                // if(note_msg.velocity != 0) {
                    // pitch_hz = mtof(note_msg.note);
                    // _voices.set_pitch(pitch_hz);
                    // _amp = (float)note_msg.velocity / 127.0;
                    // sprintf(out, "MIDI %d - HZ %.3f\n", note_msg.note, pitch_hz);
                    // sprintf(out, "MIDI %d ON VEL %d\n", note_msg.note, note_msg.velocity);
                    // SerialDebugWriteString(out, strlen(out));
                // }
                break;
            }
            case NoteOff:
            {
                synth->MidiNoteOff(msg.AsNoteOff());
                // auto note_msg = msg.AsNoteOff();
                // _amp = 0.0;
                // sprintf(out, "MIDI %d OFF VEL %d\n", note_msg.note, note_msg.velocity);
                // SerialDebugWriteString(out, strlen(out));
                break;
            }
            case MidiMessageType::ControlChange:
            {
                //auto cc_msg = msg.AsControlChange();
                synth->MidiCCProcess(msg.AsControlChange());
            }
            break;
                // Since we only care about note-on messages in this example
                // we'll ignore all other message types
            default: break;
        }
    }
}

void Hardware::synth_hardware_init() {
    synth_hw.Init();
    synth_hw.Configure();
    MidiUsbHandler::Config midi_cfg;
    midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
    synth_midi.Init(midi_cfg);
    UartHandler::Config uart_config;
    uart_config.baudrate      = 115200;
    uart_config.periph        = UartHandler::Config::Peripheral::USART_1;
    uart_config.stopbits      = UartHandler::Config::StopBits::BITS_2;
    uart_config.parity        = UartHandler::Config::Parity::NONE;
    uart_config.mode          = UartHandler::Config::Mode::TX_RX;
    uart_config.wordlength    = UartHandler::Config::WordLength::BITS_8;
    uart_config.pin_config.rx = {DSY_GPIOB, 7}; // (USART_1 RX) Daisy pin 15
    uart_config.pin_config.tx = {DSY_GPIOB, 6}; // (USART_1 TX) Daisy pin 14
    synth_uart.Init(uart_config);
    

    /** Setup timer to handle midi events */
    TimerHandle         timer5;
    TimerHandle::Config timer5_cfg;
    /** timer5 with IRQ enabled */
    timer5_cfg.periph     = TimerHandle::Config::Peripheral::TIM_5;
    timer5_cfg.enable_irq = true;
    auto tim_target_freq = TIMER5_SPEED_HZ; //Set frequency to 50hz
    auto tim_base_freq   = System::GetPClk2Freq();
    timer5_cfg.period       = tim_base_freq / tim_target_freq;
    timer5.Init(timer5_cfg);
    timer5.SetCallback(Timer5Callback);
    timer5.Start();
}

void Hardware::SynthConfig(Synth* s) {
    synth = s;
}


void Hardware::SerialDebugWriteString( const char txBuffer[],  int bufferSize){
    Hardware::synth_uart.PollTx((uint8_t *)&txBuffer[0],  bufferSize);
}