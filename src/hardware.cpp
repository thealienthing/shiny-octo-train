#include "hardware.h"
#include "Synth.h"


//MidiUsbHandler Hardware::synth_midi;
MidiUartHandler Hardware::synth_midi;
DaisySeed Hardware::synth_hw;
//UartHandler Hardware::synth_uart;
CpuLoadMeter Hardware::synth_cpu;
I2CHandle Hardware::i2c;
LCDScreen Hardware::synth_lcd(&Hardware::i2c);
Encoder Hardware::menu_knob;
int Hardware::timer5_counter = 0;
int Hardware::knob_readings[];
char Hardware::_console_out[];
bool Hardware::report_amp_env = false;
Synth* Hardware::synth;

void Hardware::Timer5Callback(void* data)
{
    //timer5_counter = (timer5_counter + 1) % ENV_PROCESS_SPEED_HZ;
    static int menu_knob_val = 0;
    static bool refresh_screen = false;
    timer5_counter = (timer5_counter + 1) % 10;
    synth_cpu.OnBlockStart();


    //Read rotary encoder
    menu_knob.Debounce();
    int reading = menu_knob.Increment();
    if(reading) {
        refresh_screen = true;
        menu_knob_val += reading;
    }

    if(timer5_counter == 0) {
        //Read potentiometers
        for(int i = 0; i < KNOB_COUNT; i++){
            int reading = synth_hw.adc.Get(i)/516;
            if(reading != knob_readings[i]) {
                refresh_screen = true;
                knob_readings[i] = reading;
            }
        }
        //CpuLoadReport();
    }
    if(report_amp_env) {
        report_amp_env = false;
    }

    if(refresh_screen){
        synth_lcd.clear();
        sprintf(_console_out, "f: %d %d %d %d %d",
            knob_readings[0], knob_readings[1], knob_readings[2],
            knob_readings[3], knob_readings[4]);
        synth_lcd.send_string(_console_out);
        //synth_lcd.clear();
        synth_lcd.put_cur(2, 0);
        sprintf(_console_out, "menu_knob: %d", menu_knob_val);
        synth_lcd.send_string(_console_out);
    }

    refresh_screen = false;

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
    //Boiler plate hardware setup
    synth_hw.Init();
    synth_hw.Configure();
    synth_hw.StartLog(false);
    
    //Set up MIDI DIN connection
    MidiUartHandler::Config midi_config;
    midi_config.transport_config.periph = UartHandler::Config::Peripheral::USART_1;
    midi_config.transport_config.tx = {DSY_GPIOB, 6}; // (USART_6 TX)
    midi_config.transport_config.rx = {DSY_GPIOB, 7}; // (USART_6 RX)
    synth_midi.Init(midi_config);
    synth_midi.StartReceive();

    //Set up CPU monitor
    synth_cpu.Init(synth_hw.AudioSampleRate(), synth_hw.AudioBlockSize());
    
    //Configure ADC channels for reading knobs
    AdcChannelConfig adc_conf[KNOB_COUNT];
    adc_conf[0].InitSingle(A0);
    adc_conf[1].InitSingle(A1);
    adc_conf[2].InitSingle(A2);
    adc_conf[3].InitSingle(A3);
    adc_conf[4].InitSingle(A4);
    synth_hw.adc.Init(adc_conf, KNOB_COUNT);
    synth_hw.adc.Start();

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

    //Menu knob encoder for navigating options on LCD
    menu_knob.Init(D1, D2, D0, 0.0f);

    //Set up I2C connection for LCD screen
    I2CHandle::Config i2c_conf;
    i2c_conf.periph = I2CHandle::Config::Peripheral::I2C_1;
    i2c_conf.speed  = I2CHandle::Config::Speed::I2C_100KHZ;
    i2c_conf.mode   = I2CHandle::Config::Mode::I2C_MASTER;
    i2c_conf.pin_config.scl  = {DSY_GPIOB, 8};
    i2c_conf.pin_config.sda  = {DSY_GPIOB, 9};
    i2c.Init(i2c_conf);
    //Send LCD initialization message
    
    synth_lcd.init();
}

void Hardware::SynthConfig(Synth* s) {
    synth = s;
}


void Hardware::SerialDebugWriteString(char txBuffer[] ){
    //Hardware::synth_uart.PollTx((uint8_t *)&txBuffer[0],  strlen(txBuffer));
}