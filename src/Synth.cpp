#include "Synth.h"

Synth::Synth(float sample_rate) {
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
    synth_midi.Listen();
    float pitch_hz = 0.0;
    char out[100];
    while(synth_midi.HasEvents())
    {
        /** Pull the oldest one from the list... */
        auto msg = synth_midi.PopEvent();
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
    synth_uart.PollTx((uint8_t *)&txBuffer[0],  bufferSize);
    
}

float Synth::mtof(int note) {
    return 440.0 * powf(2.0, (( (float)(note-69) )/12.0));
}