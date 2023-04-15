#include "hardware.h"
#include "Synth.h"


//MidiUsbHandler Hardware::synth_midi;
MidiUartHandler Hardware::synth_midi;
UartHandler Hardware::synth_console;
DaisySeed Hardware::synth_hw;
//UartHandler Hardware::synth_uart;
CpuLoadMeter Hardware::synth_cpu;
I2CHandle Hardware::i2c;
LCDScreen Hardware::synth_lcd(&Hardware::i2c);
Encoder Hardware::menu_knob;
Menu Hardware::menu;
int Hardware::timer5_counter = 0;
uint16_t Hardware::knob_readings[KNOB_COUNT][ANALOG_SAMPLE_COUNT];
uint16_t Hardware::analog_sample_index = 0;
char Hardware::_console_out[];
bool Hardware::report_amp_env = false;
Synth* Hardware::synth;

Hardware::Hardware(Synth* s) {
    synth = s;
}

void Hardware::Timer5Callback(void* data)
{
    //timer5_counter = (timer5_counter + 1) % ENV_PROCESS_SPEED_HZ;
    timer5_counter++;
    static int midi_note_exec_counter = 0;
    midi_note_exec_counter = (midi_note_exec_counter + 1) % 100;
    synth_cpu.OnBlockStart();
    static bool led_on = false;


    //Read rotary encoder
    menu_knob.Debounce();
    int reading = menu_knob.Increment();
    if(reading) {
        menu.navigate(reading);
        led_on = !led_on;
        synth_hw.SetLed(led_on);
    }
    if(menu_knob.RisingEdge()) {
        menu.select();
    }

    //Read potentiometers
    static uint64_t knob_readings[KNOB_COUNT] = { 0 };
    static uint16_t smoothed_readings[KNOB_COUNT] = { 0 };
    for(int knob = 0; knob < KNOB_COUNT; knob++){
        knob_readings[knob] += synth_hw.adc.Get(knob);
    }
    analog_sample_index += 1;
    
    if(analog_sample_index >= ANALOG_SAMPLE_COUNT){
        analog_sample_index = 0;
        for(int knob = 0; knob < KNOB_COUNT; knob++) {
            smoothed_readings[knob] = knob_readings[knob] / ANALOG_SAMPLE_COUNT;
            knob_readings[knob] = 0;
        }
        menu.update_knob_readings(smoothed_readings);
        //synth_hw.PrintLine("%lu | %lu | %lu | %lu | %lu", smoothed_readings[0], smoothed_readings[1], smoothed_readings[2], smoothed_readings[3], smoothed_readings[4]);
        
    }

    if(timer5_counter % 100 == 0) {
        //synth_hw.PrintLine("Updating menu print... %d", timer5_counter);
        menu.print_menu_params();
    }
    

    if(timer5_counter % 10 == 0) {
        double vol = LINEAR_TO_LOG(smoothed_readings[4], UINT16_MAX);
        //synth_hw.PrintLine("Master = %f", vol);
        synth->_amp = vol;
    }
    if(synth->_console_print) {
        synth->_console_print = false;
        synth_hw.PrintLine("%s", synth->_console_str);
    }

    if(MIDI_TEST_MODE) {
        static bool led_on = true;
        if(midi_note_exec_counter == 0){
            led_on = !led_on;
            PushTestMIDI();
            synth_hw.SetLed(led_on);
        }
    }
    else {
        MIDIProcess();
    }
    
    synth->AmpEnvelopeProcess();
    synth_cpu.OnBlockEnd();
    //CpuLoadReport();
}

void Hardware::ProcessSerial(uint8_t* buff) {
    //Valid Data Captured
    if(*(uint32_t*)buff == 0xdeadbeef) {
        uint32_t param_tag = *(uint32_t*)(buff+4);
        //synth_hw.PrintLine("%x | val: %f", *(uint32_t*)(buff+4), *(float*)(buff+8));

        switch (param_tag) {            
            case OSC1_WAVEFORM_SIN: {
                synth_hw.PrintLine("OSC1 SIN");
                synth->patch_params.osc1_waveform = WaveForm::Sin;
                synth->SetVoiceWaveform(Voice::Osc1, synth->patch_params.osc1_waveform);
                break;
            }
            case OSC1_WAVEFORM_SAW: {
                synth_hw.PrintLine("OSC1 SAW");
                synth->patch_params.osc1_waveform = WaveForm::Saw;
                synth->SetVoiceWaveform(Voice::Osc1, synth->patch_params.osc1_waveform);
                break;
            }
            case OSC1_WAVEFORM_SQUARE: {
                synth_hw.PrintLine("OSC1 SQUARE");
                synth->patch_params.osc1_waveform = WaveForm::Square;
                synth->SetVoiceWaveform(Voice::Osc1, synth->patch_params.osc1_waveform);
                break;
            }
            case OSC1_WAVEFORM_TRI: {
                synth_hw.PrintLine("OSC1 TRI");
                synth->patch_params.osc1_waveform = WaveForm::Tri;
                synth->SetVoiceWaveform(Voice::Osc1, synth->patch_params.osc1_waveform);
                break;
            }
            case OSC1_WAVEFORM_NOISE: {
                synth_hw.PrintLine("OSC1 NOISE");
                synth->patch_params.osc1_waveform = WaveForm::WhiteNoise;
                synth->SetVoiceWaveform(Voice::Osc1, synth->patch_params.osc1_waveform);
                break;
            }
            case OSC2_WAVEFORM_SIN: {
                synth_hw.PrintLine("OSC2 SIN");
                synth->patch_params.osc2_waveform = WaveForm::Sin;
                synth->SetVoiceWaveform(Voice::Osc2, synth->patch_params.osc2_waveform);
                break;
            }
            case OSC2_WAVEFORM_SAW: {
                synth_hw.PrintLine("OSC2 SAW");
                synth->patch_params.osc2_waveform = WaveForm::Saw;
                synth->SetVoiceWaveform(Voice::Osc2, synth->patch_params.osc2_waveform);
                break;
            }
            case OSC2_WAVEFORM_SQUARE: {
                synth_hw.PrintLine("OSC2 SQUARE");
                synth->patch_params.osc2_waveform = WaveForm::Square;
                synth->SetVoiceWaveform(Voice::Osc2, synth->patch_params.osc2_waveform);
                break;
            }
            case OSC2_WAVEFORM_TRI: {
                synth_hw.PrintLine("OSC2 TRI");
                synth->patch_params.osc2_waveform = WaveForm::Tri;
                synth->SetVoiceWaveform(Voice::Osc2, synth->patch_params.osc2_waveform);
                break;
            }
            case OSC2_WAVEFORM_NOISE: {
                synth_hw.PrintLine("OSC2 NOISE");
                synth->patch_params.osc2_waveform = WaveForm::WhiteNoise;
                synth->SetVoiceWaveform(Voice::Osc2, synth->patch_params.osc2_waveform);
                break;
            }
        }
        // synth->ApplyPatch();
    }
    buff[0] = '\0';
}

void Hardware::HardwareDebugCallback(void* data) {
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
                synth_hw.PrintLine("NoteOn -> %d", msg.AsNoteOn().note);
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

void Hardware::AudioCallback( AudioHandle::InputBuffer in,
                    AudioHandle::OutputBuffer out,
                    size_t size)
{
    //Loop through sample block size and output
    //cpuLoadMeter.OnBlockStart();
    float sample = 0.0;
    for(size_t i = 0; i < size; i++) {
        sample = synth->ProcessAudio();
        out[0][i] = sample;
        out[1][i] = sample;
    }
    //cpuLoadMeter.OnBlockEnd();
}

void Hardware::synth_hardware_init() {
    //Boiler plate hardware setup
    synth_hw.Init();
    synth_hw.Configure();
    synth_hw.StartLog(false);

    synth = new Synth(synth_hw.AudioSampleRate());
    synth_hw.StartAudio(AudioCallback);
    
    //Set up MIDI DIN connection
    MidiUartHandler::Config midi_config;
    midi_config.transport_config.periph = UartHandler::Config::Peripheral::USART_1;
    midi_config.transport_config.tx = {DSY_GPIOB, 6}; // (USART_6 TX)
    midi_config.transport_config.rx = {DSY_GPIOB, 7}; // (USART_6 RX)
    synth_midi.Init(midi_config);
    synth_midi.StartReceive();

    UartHandler::Config console_config;
    console_config.pin_config.tx = daisy::seed::D6;
    console_config.pin_config.rx = daisy::seed::D5;
    console_config.mode = UartHandler::Config::Mode::TX_RX;
    console_config.periph = UartHandler::Config::Peripheral::UART_5;
    console_config.baudrate = 115200;
    synth_console.Init(console_config);

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

    //Menu knob encoder for navigating options on LCD
    menu_knob.Init(D2, D3, D1, 0.0f);

    //Set up I2C connection for LCD screen
    I2CHandle::Config i2c_conf;
    i2c_conf.periph = I2CHandle::Config::Peripheral::I2C_1;
    i2c_conf.speed  = I2CHandle::Config::Speed::I2C_100KHZ;
    i2c_conf.mode   = I2CHandle::Config::Mode::I2C_MASTER;
    i2c_conf.pin_config.scl  = {DSY_GPIOB, 8};
    i2c_conf.pin_config.sda  = {DSY_GPIOB, 9};
    i2c.Init(i2c_conf);
    //Send LCD initialization message
    
    //Setup the lcd screen
    synth_lcd.init();
    synth_lcd.cursor_setup(true, true);
    synth_lcd.clear();

    //Then pass lcd to menu initializer

    menu = Menu();
    menu.init(&synth_lcd, synth);

    /** Setup timer to handle midi events */
    TimerHandle         timer5;
    TimerHandle::Config timer5_cfg;
    /** timer5 with IRQ enabled */
    timer5_cfg.periph     = TimerHandle::Config::Peripheral::TIM_5;
    timer5_cfg.enable_irq = true;
    auto tim_target_freq = ENV_PROCESS_SPEED_HZ; //Set frequency to 50hz
    auto tim_base_freq   = System::GetPClk2Freq();
    timer5_cfg.period       = tim_base_freq / tim_target_freq;
    synth_hw.PrintLine("tar_freq=%d | base_freq=%d | period=%d", 
        tim_target_freq,
        tim_base_freq,
        timer5_cfg.period    
    );
    timer5.Init(timer5_cfg);
    if(!HW_DEBUG_MODE){
        timer5.SetCallback(Timer5Callback);
    } 
    else{
        timer5.SetCallback(HardwareDebugCallback);
    }
    
    timer5.Start();

    load_patch1(&(synth->patch_params));
    synth->ApplyPatch();

    synth_hw.PrintLine("Initializing patchbay...");
    //initialize_flash();
    synth_hw.PrintLine("Done initializing patchbay...");

    
}

void Hardware::initialize_flash() {

    // size_t address = (size_t)flash_buffer;
    // auto flash_ptr = synth_hw.qspi.GetData(address);
    // if(flash_ptr == nullptr)
    //     return;

    // synth_hw.PrintLine("2");
    // uint8_t tag = PATCH_TAG;
    // for(int i = 0; i < 16; i++) {
    //     if(*(uint32_t*) flash_ptr != PATCH_TAG) {
    //         synth_hw.PrintLine("Writing tag to patch %d", i);
    //         synth_hw.qspi.Write(flash_ptr, __SIZEOF_INT__, &tag);
            
    //     }
    //     else {
    //         synth_hw.PrintLine("Tag already written to patch %d", i);
    //     }
    //     flash_ptr += PATCH_SIZE;
    //     address += PATCH_SIZE;
    // }
}

void Hardware::SynthConfig(Synth* s) {
    synth = s;
}


void Hardware::SerialDebugWriteString(char txBuffer[] ){
    //Hardware::synth_uart.PollTx((uint8_t *)&txBuffer[0],  strlen(txBuffer));
}

void Hardware::PushTestMIDI() {
    uint8_t midi_notes[] = {69, 73, 76, 71, 74, 78, 73, 76, 80, 68, 71, 76};
    uint8_t sequence_len = sizeof(midi_notes)/sizeof(uint8_t);
    static uint8_t index = 0;
    NoteOffEvent noteOff;
    NoteOnEvent noteOn;
    noteOff.note = midi_notes[index];
    noteOn.note = midi_notes[(index + 1) % sequence_len];
    synth->MidiNoteOff(noteOff);
    synth->MidiNoteOn(noteOn);
    index = (index + 1) % sequence_len;
}

