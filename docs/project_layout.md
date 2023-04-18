``` mermaid
classDiagram
Hardware *-- Synth
Hardware *-- Menu
Hardware *-- ADCChannel : Knob1
Hardware *-- ADCChannel : 2
Hardware *-- ADCChannel : 3
Hardware *-- ADCChannel : 4
Hardware *-- ADCChannel : 5
Hardware *-- Encoder : MenuKnob
Hardware *-- MIDIUart : MIDI
Hardware *-- USBLogger : DebugConsole

Synth *-- Voice
Synth *-- Filter
Synth *-- PatchParams
Voice *-- Oscillator : Osc1
Voice *-- Oscillator : Osc2
Menu *-- LCDScreen

class Synth{
    +Voice[NUM_VOICES] voices
    +Filter filter
}

class Filter{
    +float process()
    +void set_cutoff(uint32_t cutoffFreq)
    +void set_resonance(uint32_t gain)
}

class PatchParams{
    //Various Component Settings
}

class Voice{
    +Oscillator osc1
    +Oscillator osc2
    +Envelope amp_env
    +void set_pitch(int note)
    +void set_waveform(Osc_Number osc_num, Waveform waveform)
    +void set_osc_volume(Osc_Number, float amp)
    void set_osc2_offsetts(int note)
    +float get_sample()
}

class Oscillator{
    +Waveform waveform
    +void init(float sample_rate)
    +void set_pitch(float pitch_hz)
    +void set_waveform(Waveform waveform)
    +float get_sample()
}

class Hardware{
    +Synth* synth
    +Menu  menu
}

class ADCChannel{
    +uint16_t Get()
}

class Menu{
    +LCDScreen* lcd;
    +Synth* synth;
    +void navigate(int reading)
    +void select()
    +void update_knob_readings(const uint16_t new_readings[KNOB_COUNT])
    +void print_menu_params()
    //Various menu print functions
}

class LCDScreen{
    I2CHandle* i2c
    init()
    send_string (char *str)
    put_cur(int row, int col) 
    cursor_setup(bool cur_on, bool blink_on)
    clear()
}

class Encoder{
    +bool RisingEdge()
    +int increment()
}

class MIDIUart{
    +void Listen()
    +bool HasEvents()
    +MidiEvent PopEvent()
}




```