#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <stdint.h>
#include "hardware.h"

#define ENVELOPE_MAX_TIME 5

class Envelope {
public:
    //Enum for tracking what action is happening within the envelope
    enum Phase {
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE,
        TAIL_OFF //For when note is changing before a release can be completed (when not enough voices)
    };

    Hardware hw;

    Envelope() {}
    ~Envelope() {}
    float val = 0.0;
    bool trigger_on = false;
    Phase phase = Phase::ATTACK;
    float process();
    void trigger();
    void reset();
    void set_attack(uint8_t attack);
    void set_decay(uint8_t decay);
    void set_sustain(uint8_t sustain);
    void set_release(uint8_t release);
     
    
private:

    

    //Private setters for these variables since they will trigger a change in other variables
    float _attack = 0;
    float _decay = 0;
    float _sustain = 0;
    float _release = 0;

    //Can be reset by hardware interface which will reconfigure the timer
    static uint32_t _sample_rate; //hz
    
};

#endif