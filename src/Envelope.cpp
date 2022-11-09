#include "Envelope.h"

float Envelope::_attack = 0.1;
float Envelope::_decay = 0.1;
float Envelope::_sustain = 0.1;
float Envelope::_release = 0.1;

void Envelope::gate_set(bool set) {
    trigger_on = set;
    if(trigger_on) {
        phase = Phase::ATTACK;
        val = 0.0;
    }
}

void Envelope::reset() {
    trigger_on = false;
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
    else if(phase == Phase::SUSTAIN) {
        if(!trigger_on) {
            phase = Phase::RELEASE;
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
    _attack = 1.0 / ((attack/127.0)*TIMER5_SPEED_HZ*ENVELOPE_MAX_TIME);
    //Calculate how much to increase amp volume per millisecond
    Hardware::report_amp_env = true;
}

void Envelope::set_decay(uint8_t decay) {
    _decay = 1.0 / ((decay/127.0)*TIMER5_SPEED_HZ*ENVELOPE_MAX_TIME);
    Hardware::report_amp_env = true;
}

void Envelope::set_sustain(uint8_t sustain) {
    _sustain = sustain/127.0;
    Hardware::report_amp_env = true;
}

void Envelope::set_release(uint8_t release) {
    _release = 1.0 / ((release/127.0)*TIMER5_SPEED_HZ*ENVELOPE_MAX_TIME);
    Hardware::report_amp_env = true;
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