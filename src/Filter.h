#ifndef FILTER_H
#define FILTER_H

#include <cmath>
#include "utils.h"

enum FilterType {
    LowPass24db,
    LowPass12db,
    BandPass12db,
    HighPass12db,
    FilterTypeEnd
};

class LowPassFilter {
public:
    LowPassFilter() {};
    LowPassFilter(float sampleRate, float cutoffFrequency, float q);

    float process(float input);
    void set_cutoff(uint32_t freq_hz);
    void set_resonance(uint32_t gain);
    void update_coefs();

    float cutoffFreq;
    float resonance;
private:
    float sampleRate;
    float b0, b1, b2, a0, a1, a2;
    float x1, x2, y1, y2;
};

#endif