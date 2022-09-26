#include <cmath>
#include "utils.h"
class Oscillator {
public:
    void init(float sample_rate);
    void set_pitch(float pitch_hz);
    float get_sample();
private:
    float _sample_rate = 0.0;
    float _pitch = 0.0;
    float _phase = 0.0;
    float _phase_delta = 0.0;
};