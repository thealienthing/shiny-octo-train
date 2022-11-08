#include "Envelope.h"

void Envelope::trigger() {
    trigger_on = true;
    phase = Phase::ATTACK;
    val = 0.0;
}

void Envelope::reset() {
    trigger_on = false;
    val = 0.0;
}

float Envelope::process() {
    if(phase == Phase::ATTACK) {
        val += _attack;
        if(val >= 1.0) {
            phase = Phase::RELEASE;
        }
    }
    else if(phase == Phase::RELEASE) {
        val -= _release;
        if(val <= 0.0) {
            phase = Phase::ATTACK;
        }
    }

    return val;

}

void Envelope::set_attack(uint8_t attack) {

    //Calculate how much to increase amp volume per millisecond
    _attack = 1.0 / ((attack/127.0)*TIMER5_SPEED_HZ*ENVELOPE_MAX_TIME);
}

void Envelope::set_decay(uint8_t decay) {
    _decay = decay;
}

void Envelope::set_sustain(uint8_t sustain) {
    _sustain = sustain;
}

void Envelope::set_release(uint8_t release) {
    _release = 1.0 / ((release/127.0)*TIMER5_SPEED_HZ*ENVELOPE_MAX_TIME);
}

uint32_t Envelope::_sample_rate = 1000;