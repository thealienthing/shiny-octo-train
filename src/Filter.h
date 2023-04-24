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

/*
    This filter class is an implementation of a second order filter using the difference equation.
*/

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
    float b0, b1, b2, a0, a1, a2; // Filter coeficients are recalculated everytime the cutoff or resonance are changes to the filter
    float x1, x2, y1, y2;   // x1 and x2 represent the input signal from one and two process cycles previous
                            //y1 and y2 represent the output signal from one and two precess cycles previous
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