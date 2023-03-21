#ifndef PATCH_PARAMS_H
#define PATCH_PARAMS_H

#include "Oscillator.h"
#include "Filter.h"

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
    int osc2_tune = 0; //hz 


    //mixer menu
    float oscillator1_level = 0.5;
    float oscillator2_level = 0.5;
    float noise_level = 0.0;

    //filter menu
    int filter_type = 0;
    uint32_t filter_cutoff = 100; // Percent of frequencies allowed to pass
    float filter_resonance = 1.0;
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

inline void load_patch1(PatchParams* params) {
    //voice menu
    params->oscillator_count = 2;
    params->voice_assign = 0;
    params->trigger_mode = 0;
    params->unison_detune = 0;

    //pitch menu
    params->transpose = 0;
    params->tune = 0.0;
    params->portamento = 0;
    params->bend_range = 2;
    params->vibrato_intensity = 0.0;

    //oscillators menu
    params->osc1_waveform = WaveForm::Sin;
    params->osc2_waveform = WaveForm::Square;
    params->osc2_semitone = 0;
    params->osc2_tune = 0; //hz 


    //mixer menu
    params->oscillator1_level = 0.5;
    params->oscillator2_level = 0.5;
    params->noise_level = 0.0;

    //filter menu
    params->filter_type = 0;
    params->filter_cutoff = 1000; // Percent of frequencies allowed to pass
    params->filter_resonance = 1.0;
    params->filter_env_intensity = 0.5;

    //filter envelope menu
    params->filter_env_attack = 10; //ms
    params->filter_env_decay = 100; //ms
    params->filter_env_sustain = 1.0; //how open filter after decay
    params->filter_env_release = 100; //ms

    //amp menu
    params->level = 0.5; //master volume out
    params->panpot = 0; //idk what this is look at microKorg lol
    params->distortion = 0.0; //gain
    params->kdb_track = 0; //also have no idea what this is

    //amp env menu
    params->amp_env_attack = 50; //ms
    params->amp_env_decay = 100; //ms
    params->amp_env_sustain = 1.0; //how high gain after decay
    params->amp_env_release = 500; //ms

    //lfo1 menu
    params->lfo1_wave = 0; //lfo waveform
    params->lfo1_keysync = false; //lfo syncs with gate from keyboard
    params->lfo1_tempsync = false; //lfo frequency will sync with internal metronome
    params->lfo1_frequency = 100; //hz. should be a beat division if tempsync is true

    //lfo2 menu
    params->lfo2_wave = 0; //lfo waveform
    params->lfo2_keysync = false; //lfo syncs with gate from keyboard
    params->lfo2_tempsync = false; //lfo frequency will sync with internal metronome
    params->lfo2_frequency = 100; //hz. should be a beat division if tempsync is true
}

#endif