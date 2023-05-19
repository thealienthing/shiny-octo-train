#include "Envelope.h"

void Envelope::note_on() {
    phase = Phase::ATTACK;
    val = 0.0;
    
}

void Envelope::note_off() {
    phase = Phase::RELEASE;
    _release = val / _release_ms;
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
    else if(phase == Phase::SUSTAIN) {
        
        
    }
    else if(phase == Phase::TAIL_OFF) {
        val -= 0.2;
        if(val <= 0.0) {
            reset();
            
        }
    }
    return val;

}

void Envelope::set_attack(uint16_t attack) {
    //_attack_ms = (attack/127.0) * ENV_PROCESS_SPEED_HZ*ENVELOPE_MAX_TIME_SEC;
    _attack_ms = attack / 4;
    _attack = 1.0 / _attack_ms;
    //Calculate how much to increase amp volume per millisecond
    //
}

void Envelope::set_decay(uint16_t decay) {
    //_decay_ms = (decay/127.0) * ENV_PROCESS_SPEED_HZ*ENVELOPE_MAX_TIME_SEC;
    _decay_ms = decay / 4;
    _decay = 1.0 / _decay_ms;
    //
}

void Envelope::set_sustain(float sustain) {
    // _sustain = (float)sustain / (float)UINT16_MAX; //This line seems like a mistake. I should be passing the normalized value
    _sustain = sustain;
    //If sustain has changed, we need to update decay time
    //set_decay(_decay);
    //
}

void Envelope::set_release(uint16_t release) {
    //_release_ms = (release/127.0) * ENV_PROCESS_SPEED_HZ*ENVELOPE_MAX_TIME_SEC;
    _release_ms = release / 4;
    _release = 1.0 / _release_ms;
    // _release = 1.0 / _release_ms;
    // _release = val / _release_ms;
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