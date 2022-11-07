#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <stdint.h>

class Envelope {
    //Private enum for tracking what action is happening within the envelope
    enum Phase {
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE,
        TAIL_OFF //For when note is changing before a release can be completed (when not enough voices)
    };

public:
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
    uint8_t _attack = 0;
    uint8_t _decay = 0;
    uint8_t _sustain = 0;
    uint8_t _release = 0;

    //Can be reset by hardware interface which will reconfigure the timer
    static uint32_t _sample_rate; //hz
    
};

#endif