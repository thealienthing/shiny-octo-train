#include "hardware.h"
#include "Synth.h"


//MidiUsbHandler Hardware::synth_midi;
MidiUartHandler Hardware::synth_midi;
DaisySeed Hardware::synth_hw;
//UartHandler Hardware::synth_uart;
CpuLoadMeter Hardware::synth_cpu;
I2CHandle Hardware::i2c;
int Hardware::timer5_counter = 0;
char Hardware::_console_out[];
bool Hardware::report_amp_env = false;
Synth* Hardware::synth;

void Hardware::Timer5Callback(void* data)
{
    timer5_counter = (timer5_counter + 1) % ENV_PROCESS_SPEED_HZ;
    synth_cpu.OnBlockStart();
    //bool led = (System::GetNow() & 1023) > 511;
    //synth_hw.SetLed(led);
    if(timer5_counter == 0) {
        //CpuLoadReport();
        //synth_hw.PrintLine("Tick...");
    }
    if(report_amp_env) {
        //sprintf(_console_out, "A=%f D=%f S=%f R=%f\n", Envelope::get_attack(), Envelope::get_decay(), Envelope::get_sustain(), Envelope::get_release());
        synth->PrintVoiceInfo(7);
        //SerialDebugWriteString(_console_out);
        report_amp_env = false;
    }

    MIDIProcess();
    synth->AmpEnvelopeProcess();
    synth_cpu.OnBlockEnd();
    //CpuLoadReport();
}

void Hardware::CpuLoadReport() {
    const float avgLoad = synth_cpu.GetAvgCpuLoad();
    const float maxLoad = synth_cpu.GetMaxCpuLoad();
    const float minLoad = synth_cpu.GetMinCpuLoad();
    // print it to the serial connection (as percentages)
    sprintf(_console_out, "====CPU Load====\n");
    //SerialDebugWriteString(_console_out);
    sprintf(_console_out, "Max: %f\n", (maxLoad * 100.0f));
    //SerialDebugWriteString(_console_out);
    sprintf(_console_out, "Avg: %f\n", (avgLoad * 100.0f));
    //SerialDebugWriteString(_console_out);
    sprintf(_console_out, "Min: %f\n", (minLoad * 100.0f));
    //SerialDebugWriteString(_console_out);
}

void Hardware::MIDIProcess() {
    synth_midi.Listen();
    while(synth_midi.HasEvents())
    {
        
        // Pull the oldest one from the list...
        auto msg = synth_midi.PopEvent();
        //auto msg = MidiEvent();
        switch(msg.type)
        {
            case NoteOn:
            {
                synth_hw.SetLed(true);
                //LCD_SetScreen("NoteOn!");
                synth->MidiNoteOn(msg.AsNoteOn());
                //and change the frequency of the oscillator 
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
                synth_hw.SetLed(false);
                //LCD_SetScreen("NoteOff!");
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
    synth_hw.StartLog(false);
    
    //MidiHandler<MidiUartTransport>::Config midi_cfg;
    
    MidiUartHandler::Config midi_config;
    midi_config.transport_config.periph = UartHandler::Config::Peripheral::USART_1;
    midi_config.transport_config.tx = {DSY_GPIOB, 6}; // (USART_6 TX)
    midi_config.transport_config.rx = {DSY_GPIOB, 7}; // (USART_6 RX)
    synth_midi.Init(midi_config);
    synth_midi.StartReceive();
    
    
    
    //MidiUsbHandler::Config midi_cfg;
    //midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
    //synth_midi.Init(midi_cfg);
    //MidiUartTransport::Config midi_cfg;
    //midi_cfg.periph = UartHandler::Config::Peripheral::USART_1;
    //midi_cfg.tx = {DSY_GPIOB, 6}; // (USART_1 TX) Daisy pin 14
    //midi_cfg.rx = {DSY_GPIOB, 7}; // (USART_1 RX) Daisy pin 15
    //synth_midi = MidiUartHandler<midi_transport>;
    
    
    // MidiUartHandler::Config uart_config;
    // uart_config.
    // uart_config.baudrate      = 115200;
    // uart_config.periph        = UartHandler::Config::Peripheral::USART_1;
    // uart_config.stopbits      = UartHandler::Config::StopBits::BITS_2;
    // uart_config.parity        = UartHandler::Config::Parity::NONE;
    // uart_config.mode          = UartHandler::Config::Mode::TX_RX;
    // uart_config.wordlength    = UartHandler::Config::WordLength::BITS_8;
    // uart_config.pin_config.rx = {DSY_GPIOB, 7}; // (USART_1 RX) Daisy pin 15
    // uart_config.pin_config.tx = {DSY_GPIOB, 6}; // (USART_1 TX) Daisy pin 14
    // synth_midi.Init(uart_config);
    //synth_uart.Init(uart_config);

    synth_cpu.Init(synth_hw.AudioSampleRate(), synth_hw.AudioBlockSize());
    

    /** Setup timer to handle midi events */
    TimerHandle         timer5;
    TimerHandle::Config timer5_cfg;
    /** timer5 with IRQ enabled */
    timer5_cfg.periph     = TimerHandle::Config::Peripheral::TIM_5;
    timer5_cfg.enable_irq = true;
    auto tim_target_freq = ENV_PROCESS_SPEED_HZ; //Set frequency to 50hz
    auto tim_base_freq   = System::GetPClk2Freq();
    timer5_cfg.period       = tim_base_freq / tim_target_freq;
    timer5.Init(timer5_cfg);
    timer5.SetCallback(Timer5Callback);
    timer5.Start();

    // setup the configuration
    
    I2CHandle::Config i2c_conf;
    i2c_conf.periph = I2CHandle::Config::Peripheral::I2C_1;
    i2c_conf.speed  = I2CHandle::Config::Speed::I2C_100KHZ;
    i2c_conf.mode   = I2CHandle::Config::Mode::I2C_MASTER;
    i2c_conf.pin_config.scl  = {DSY_GPIOB, 8};
    i2c_conf.pin_config.sda  = {DSY_GPIOB, 9};
    // initialise the peripheral
    i2c.Init(i2c_conf);
    lcd_init(&i2c);
    
    // now i2c points to the corresponding peripheral and can be used.
}

void Hardware::LCD_SetScreen(char* str) {
    lcd_clear(&i2c);
    lcd_put_cur(&i2c, 0, 0);
    lcd_send_string(&i2c, str);
}

void Hardware::SynthConfig(Synth* s) {
    synth = s;
}


void Hardware::SerialDebugWriteString(char txBuffer[] ){
    //Hardware::synth_uart.PollTx((uint8_t *)&txBuffer[0],  strlen(txBuffer));
}