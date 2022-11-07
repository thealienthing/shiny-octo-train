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
        val += 0.00001;
        if(val >= 1.0) {
            phase = Phase::RELEASE;
        }
    }
    else if(phase == Phase::RELEASE) {
        val -= 0.00002;
        if(val <= 0.0) {
            phase = Phase::ATTACK;
        }
    }

    return val;

}

void Envelope::set_attack(uint8_t attack) {
    _attack = attack;
}

void Envelope::set_decay(uint8_t decay) {
    _decay = decay;
}

void Envelope::set_sustain(uint8_t sustain) {
    _sustain = sustain;
}

void Envelope::set_release(uint8_t release) {
    _release = release;
}

uint32_t Envelope::_sample_rate = 1000;