#ifndef PATCH_PARAMS_H
#define PATCH_PARAMS_H

#include "Oscillator.h"

struct PatchParams {
    //voice menu
    int oscillator_count = 2;
    int voice_assign = 0;
    int trigger_mode = 0;
    int unison_detune = 0;

    //pitch menu
    int transpose = 0;
    float tune = 0.0;
    int portamento = 0;
    int bend_range = 2;
    float vibrato_intensity = 0.0;

    //oscillators menu
    WaveForm osc1_waveform = WaveForm::Saw;
    WaveForm osc2_waveform = WaveForm::Saw;
    int osc2_semitone = 0;
    float osc2_tune = 0.0;


    //mixer menu
    float oscillator1_level = 0.5;
    float oscillator2_level = 0.5;
    float noise_level = 0.0;

    //filter menu
    int filter_type = 0;
    int cutoff = 20000;
    float resonance = 1.0;
    float filter_env_intensity = 0.5;

    //filter envelope menu
    int filter_env_attack = 10; //ms
    int filter_env_decay = 100; //ms
    float filter_env_sustain = 1.0; //how open filter after decay
    int filter_env_release = 100; //ms

    //amp menu
    float level = 0.5; //master volume out
    int panpot = 0; //idk what this is look at microKorg lol
    float distortion = 0.0; //gain
    int kdb_track = 0; //also have no idea what this is

    //amp env menu
    int amp_env_attack = 10; //ms
    int amp_env_decay = 100; //ms
    float amp_env_sustain = 1.0; //how high gain after decay
    int amp_env_release = 100; //ms

    //lfo1 menu
    int lfo1_wave = 0; //lfo waveform
    bool lfo1_keysync = false; //lfo syncs with gate from keyboard
    bool lfo1_tempsync = false; //lfo frequency will sync with internal metronome
    int lfo1_frequency = 100; //hz. should be a beat division if tempsync is true

    //lfo2 menu
    int lfo2_wave = 0; //lfo waveform
    bool lfo2_keysync = false; //lfo syncs with gate from keyboard
    bool lfo2_tempsync = false; //lfo frequency will sync with internal metronome
    int lfo2_frequency = 100; //hz. should be a beat division if tempsync is true
};

#endif