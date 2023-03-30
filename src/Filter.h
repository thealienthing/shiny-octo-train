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

class Filter {
public:
    Filter() {};
    Filter(float sampleRate, float cutoffFrequency, float q);
    virtual float process(float input) = 0;
    void set_cutoff(uint32_t freq_hz);
    void set_resonance(uint32_t gain);
    virtual void update_coefs() = 0;

    float cutoffFreq;
    float resonance;
    
protected:
    float sampleRate;
    float b0, b1, b2, a0, a1, a2;
    float x1, x2, y1, y2;
};

class LowPassFilter : public Filter {
public:
    LowPassFilter() {};
    LowPassFilter(float sampleRate, float cutoffFrequency, float q);

    float process(float input);
    void update_coefs();
};

class HighPassFilter : public Filter {
public:
    HighPassFilter() {};
    HighPassFilter(float sampleRate, float cutoffFrequency, float q);

    float process(float input);
    void update_coefs();
};

#endif