#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <stdint.h>
#include "hardware.h"

#define ENVELOPE_MAX_TIME_SEC 5

class Envelope {
public:
    //Enum for tracking what action is happening within the envelope
    enum Phase {
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE,
        TAIL_OFF, //For when note is changing before a release can be completed (when not enough voices)
        READY
    };

    Hardware hw;

    Envelope() {}
    ~Envelope() {}
    float val = 0.0;
    Phase phase = Phase::READY;
    float process();
    void note_on();
    void note_off();
    void reset();

    static float get_attack();
    static float get_decay();
    static float get_sustain();
    static float get_release();
    static void set_attack(uint8_t attack);
    static void set_decay(uint8_t decay);
    static void set_sustain(uint8_t sustain);
    static void set_release(uint8_t release);
    
private:
    //Private setters for these variables since they will trigger a change in other variables
    static uint16_t _attack_ms, _decay_ms, _release_ms;
    static float _attack;
    static float _decay;
    static float _sustain;
    static float _release;

    //Can be reset by hardware interface which will reconfigure the timer
    static uint32_t _sample_rate; //hz
    
};

#endif