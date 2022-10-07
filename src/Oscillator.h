#include <cmath>
#include "utils.h"

enum WaveForm {
    Sin,
    Tri,
    Saw,
    Square,
    Noise
};

class Oscillator {
public:
    void init(float sample_rate);
    void set_pitch(float pitch_hz);
    void set_waveform(WaveForm waveform);
    float get_sample();

    
    
private:
    WaveForm _waveform = WaveForm::Sin;
    float _sample_rate = 0.0;
    float _pitch = 0.0;
    float _pitch_pending = 0.0;
    float _phase = 0.0;
    float _phase_delta = 0.0;
    const float _half_cycle = TWO_PI * 0.5;
    bool _crossing_zero = false;
    bool _changing_pitch = false;
};