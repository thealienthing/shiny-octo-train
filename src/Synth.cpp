#include "Synth.h"
#include "midi_map.h"
#include "hardware.h"

Synth::Synth(float sample_rate) {
    //cpuLoadMeter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());
    _sample_rate = sample_rate;
    for(uint8_t i = 0; i < sizeof(_voices)/sizeof(Voice); i++) {
        _voices[i].init(_sample_rate);
        _voices[i].set_waveform(Voice::Osc_Number::Osc1, _osc1_wf);
        _voices[i].set_waveform(Voice::Osc_Number::Osc2, _osc2_wf);
    }
    
}

float Synth::ProcessAudio() {
    float sample = 0.0;
    //float voice_balance = 1.0 / _voice_count;
    for(uint8_t i = 0; i < _voice_count; i++) {
       sample += _voices[i].get_sample();// * voice_balance;
    }
    return sample * _amp;
}

void Synth::AmpEnvelopeSet(Envelope::Phase phase, uint8_t val) {
    sprintf(_console_str, "AMP ENV PHASE %d = %d\n", (int)phase, val);
    hw.SerialDebugWriteString(_console_str, strlen(_console_str));
    for(uint8_t i = 0; i < NUM_VOICES; i++) {
        if(phase == Envelope::Phase::ATTACK)
            _voices[i].amp_env.set_attack(val);
        else if(phase == Envelope::Phase::DECAY)
            _voices[i].amp_env.set_decay(val);
        else if(phase == Envelope::Phase::SUSTAIN)
            _voices[i].amp_env.set_sustain(val);
        else if(phase == Envelope::Phase::RELEASE)
            _voices[i].amp_env.set_release(val);
    }
}

void Synth::AmpEnvelopeProcess() {
    for(uint8_t i = 0; i < NUM_VOICES; i++) {
        if(_voices[i].amp_env.trigger_on) {
            _voices[i].amp_env.process();
        }
    }
}

void Synth::MidiCCProcess(ControlChangeEvent event) {
    switch(event.control_number) {

        //Oscillator Controls, waveform, mix level etc...
        case CC_OSC1_MIX_LEVEL: {
            for(int i = 0; i < NUM_VOICES; i++) {
                float volume = sqrtf((float)event.value/127.0);
                _voices[i].set_osc_volume(Voice::Osc1, volume*0.5);
            }
            break;
        }
        case CC_OSC2_MIX_LEVEL: {
            for(int i = 0; i < NUM_VOICES; i++) {
                float volume = sqrtf((float)event.value/127.0);
                _voices[i].set_osc_volume(Voice::Osc2, volume*0.5);
            }
            break;
        }
        case CC_OSC1_WAVE_FORM: {
            auto sel = event.value / (127 / WAVEFORMS_TOTAL);
            if (sel >= WAVEFORMS_TOTAL) sel--;
            if ((WaveForm) sel != _osc1_wf) {
                _osc1_wf = (WaveForm)sel;
                sprintf(_console_str, "WAVE SEL = %d\n", sel);
                hw.SerialDebugWriteString(_console_str, strlen(_console_str));
                SetVoiceWaveform(Voice::Osc_Number::Osc1, _osc1_wf);
            }
            break;
        }
        case CC_OSC2_WAVE_FORM: {
            auto sel = event.value / (127 / WAVEFORMS_TOTAL);
            if (sel >= WAVEFORMS_TOTAL) sel--;
            if ((WaveForm) sel != _osc2_wf) {
                _osc2_wf = (WaveForm)sel;
                sprintf(_console_str, "WAVE SEL = %d\n", sel);
                hw.SerialDebugWriteString(_console_str, strlen(_console_str));
                SetVoiceWaveform(Voice::Osc_Number::Osc2, _osc2_wf);
            }
            break;
        }

        //Amp and filter envelope controls
        case CC_AMP_ENV_ATTACK: {
            AmpEnvelopeSet(Envelope::Phase::ATTACK, event.value);
            break;
        }
        case CC_AMP_ENV_DECAY: {
            AmpEnvelopeSet(Envelope::Phase::DECAY, event.value);
            break;
        }
        case CC_AMP_ENV_SUSTAIN: {
            AmpEnvelopeSet(Envelope::Phase::SUSTAIN, event.value);
            break;
        }
        case CC_AMP_ENV_RELEASE: {
            AmpEnvelopeSet(Envelope::Phase::RELEASE, event.value);
            break;
        }
        case CC_FILTER_ENV_ATTACK: {
            break;
        }
        case CC_FILTER_ENV_DECAY: {
            break;
        }
        case CC_FILTER_ENV_SUSTAIN: {
            break;
        }
        case CC_FILTER_ENV_RELEASE: {
            break;
        }
    }
    //sprintf(_console_str, "CC MSG: CHAN %d | CTRL NUM %d | CC VAL %d\n",
    //    event.channel, event.control_number, event.value);
    //hw.SerialDebugWriteString(_console_str, strlen(_console_str));
}

void Synth::MidiNoteOn(NoteOnEvent event) {
    //sprintf(_console_str, "MIDI %d ON VEL %d\n", event.note, event.velocity);
    //SerialDebugWriteString(_console_str, strlen(_console_str));
    if(_voice_count == NUM_VOICES) {
        sprintf(_console_str, "Note %d! Max voices. Removing oldest voice\n", event.note);
        hw.SerialDebugWriteString(_console_str, strlen(_console_str));
        for(int i = 0; i < NUM_VOICES-1; i++){
            _voice_map[i] = _voice_map[i+1];
            _voices[i].set_pitch(mtof(_voice_map[i].note));
            _voices[i].amp_env.trigger();
        }
        _voice_map[NUM_VOICES-1] = event;
    }
    else {
        _voice_map[_voice_count] = event;
        _voices[_voice_count].set_pitch(mtof(event.note));
        _voices[_voice_count].amp_env.trigger();
        _voice_count++;
        sprintf(_console_str, "Note %d! Voice count: %d\n", event.note, _voice_count);
        hw.SerialDebugWriteString(_console_str, strlen(_console_str));
    }
    PrintVoiceMap();
}

void Synth::MidiNoteOff(NoteOffEvent event) {
    //sprintf(_console_str, "MIDI %d OFF VEL %d\n", event.note, event.velocity);
    //SerialDebugWriteString(_console_str, strlen(_console_str));
    if(_voice_count > 0) {
        int i = 0;
        //sprintf(_console_str, "Note off! Voice count: %d\n", _voice_count);
        //SerialDebugWriteString(_console_str, strlen(_console_str));
        for(; i < NUM_VOICES; i++) {
            if(_voice_map[i].note == event.note){
                //_voice_map[i] = NoteOnEvent();
                break;
            }
        }
        for(; i < _voice_count-1; i++) {
            _voice_map[i] = _voice_map[i+1];
            _voices[i].set_pitch(mtof(_voice_map[i].note));
        }
        _voices[_voice_count-1].amp_env.reset();
        _voice_map[_voice_count-1] = NoteOnEvent();
        _voice_count--;
    }
    PrintVoiceMap();
}

void Synth::PrintVoiceMap() {
    
    hw.SerialDebugWriteString("VoiceMap[", 9);
    for(int i = 0; i < NUM_VOICES; i++){
        sprintf(_console_str, "%d,", _voice_map[i].note);
        hw.SerialDebugWriteString(_console_str, strlen(_console_str));
    }
    hw.SerialDebugWriteString("]\n", 2);
}

void Synth::SetVoiceWaveform(Voice::Osc_Number osc, WaveForm waveform) {
    for(int i = 0; i < NUM_VOICES; i++){
        _voices[i].set_waveform(osc, waveform);
    }
}

float Synth::mtof(int note) {
    return 440.0 * powf(2.0, (( (float)(note-69) )/12.0));
}