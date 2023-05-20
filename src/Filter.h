#ifndef FILTER_H
#define FILTER_H

#include <cmath>
#include "utils.h"

enum FilterType {
    LowPass,
    HighPass,
    BandPass,
    FilterTypeEnd
};

/*
    This filter class is an implementation of a second order filter using the difference equation.
*/

class Filter {
public:
    Filter() {};
    Filter(FilterType filter_type, float sampleRate, float cutoffFrequency, float q);
    float process(float input);
    void set_cutoff(uint32_t freq_hz);
    void set_resonance(uint32_t gain);
    void update_coefs();
    void set_filter_type(FilterType filterType);

    float cutoffFreq;
    float resonance;
    FilterType filterType;
    
protected:
    float sampleRate;
    float b0, b1, b2, a0, a1, a2; // Filter coeficients are recalculated everytime the cutoff or resonance are changes to the filter
    float x1, x2, y1, y2;   // x1 and x2 represent the input signal from one and two process cycles previous
                            //y1 and y2 represent the output signal from one and two precess cycles previous
};

#endif