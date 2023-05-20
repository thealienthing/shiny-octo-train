#include "Filter.h"

void Filter::set_cutoff(uint32_t freq_hz) {
    cutoffFreq = freq_hz;
    update_coefs();
}

void Filter::set_resonance(uint32_t gain) {
    resonance = gain;
    update_coefs();
}

void Filter::set_filter_type(FilterType filter_type) {
    filterType = filter_type;
    update_coefs();
}

Filter::Filter(FilterType filter_type, float sample_rate, float cutoffFrequency, float q) {
    filterType = filter_type;
    sampleRate = sample_rate;
    cutoffFreq = cutoffFrequency;
    resonance = q;
    x1 = x2 = y1 = y2 = 0.0f;
    update_coefs();
}

void Filter::update_coefs() {
    //Normalized cutoff frequency in radians per sample in digital domain.
    float cutoff_norm = 2.0f * M_PI * cutoffFreq / sampleRate;

    //Applying cosin function to the normalized cutoff frequency to simplify
    //the expressions for the bilinear transform
    float cutoff_cos = std::cos(cutoff_norm);

    //freq_response_shape is refers to the shape of the frequency response
    //in the filter as well as resonance boost around the cutoff frequency
    float freq_response_shape = sin(cutoff_norm) / (2.0f * resonance);

    //A couple of steps in one here performing the bilinear transform
    //We perform the bilinear transform by substituting the continuous time
    //transfer function of the frequency cutoff to a transfer function of
    //the frequency response in discrete time domain

    switch (filterType)
    {
        case FilterType::LowPass: {
            b0 = (1.0f - cutoff_cos) / 2.0f;
            b1 = 1.0f - cutoff_cos;
            b2 = (1.0f - cutoff_cos) / 2.0f;
            a0 = 1.0f + freq_response_shape;
            a1 = -2.0f * cutoff_cos;
            a2 = 1.0f - freq_response_shape;        
            break;
        }
        case FilterType::HighPass: {
            b0 = (1.0f + cutoff_cos) / 2.0f;
            b1 = -(1.0f + cutoff_cos);
            b2 = (1.0f + cutoff_cos) / 2.0f;
            a0 = 1.0f + freq_response_shape;
            a1 = -2.0f * cutoff_cos;
            a2 = 1.0f - freq_response_shape;
            break;
        }
        default:
            break;
    }
}


float Filter::process(float input) {
    float output = (b0 / a0) * input + (b1 / a0) * x1 + (b2 / a0) * 
        x2 - (a1 / a0) * y1 - (a2 / a0) * y2;
    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = output;

    return output;
}