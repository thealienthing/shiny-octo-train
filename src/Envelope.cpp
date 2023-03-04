#include "Envelope.h"

float Envelope::_attack = 0.5;
float Envelope::_decay = 0.1;
float Envelope::_sustain = 0.1;
float Envelope::_release = 0.1;
uint16_t Envelope::_attack_ms = 0.1;
uint16_t Envelope::_decay_ms = 0.1;
uint16_t Envelope::_release_ms = 0.1;

void Envelope::note_on() {
    phase = Phase::ATTACK;
    val = 0.0;
    
}

void Envelope::note_off() {
    phase = Phase::RELEASE;
    
}

void Envelope::reset() {
    val = 0.0;
    phase = Phase::READY;
    
}

float Envelope::process() {
    if(phase == Phase::ATTACK) {
        val += _attack;
        if(val >= 1.0) {
            phase = Phase::DECAY;
            
        }
    }
    else if(phase == Phase::DECAY) {
        val -= _decay;
        if(val <= _sustain) {
            phase = Phase::SUSTAIN;
            
        }
    }
    else if(phase == Phase::RELEASE) {
        val -= _release;
        if(val <= 0.0) {
            reset();
            
        }
    }
    else if(phase == Phase::TAIL_OFF) {
        val -= 0.2;
        if(val <= 0.0) {
            reset();
            
        }
    }
    return val;

}

void Envelope::set_attack(uint8_t attack) {
    _attack_ms = (attack/127.0) * ENV_PROCESS_SPEED_HZ*ENVELOPE_MAX_TIME_SEC;
    _attack = 1.0 / _attack_ms;
    //Calculate how much to increase amp volume per millisecond
    //
}

void Envelope::set_decay(uint8_t decay) {
    _decay_ms = (decay/127.0) * ENV_PROCESS_SPEED_HZ*ENVELOPE_MAX_TIME_SEC;
    _decay = (1.0-_sustain) / _decay_ms;
    //
}

void Envelope::set_sustain(uint8_t sustain) {
    _sustain = sustain/127.0;
    //If sustain has changed, we need to update decay time
    set_decay(_decay);
    //
}

void Envelope::set_release(uint8_t release) {
    _release_ms = (release/127.0) * ENV_PROCESS_SPEED_HZ*ENVELOPE_MAX_TIME_SEC;
    _release = _sustain / _release_ms;
    //
}

float Envelope::get_attack() {
    return _attack;
}
float Envelope::get_decay() {
    return _decay;
}
float Envelope::get_sustain() {
    return _sustain;
}
float Envelope::get_release() {
    return _release;
}
uint32_t Envelope::_sample_rate = 1000;