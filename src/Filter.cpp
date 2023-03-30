#include "Filter.h"

void Filter::set_cutoff(uint32_t freq_hz) {
    cutoffFreq = freq_hz;
    update_coefs();
}

void Filter::set_resonance(uint32_t gain) {
    resonance = gain;
    update_coefs();
}

LowPassFilter::LowPassFilter(float sample_rate, float cutoffFrequency, float q) {
    sampleRate = sample_rate;
    cutoffFreq = cutoffFrequency;
    resonance = q;
    x1 = x2 = y1 = y2 = 0.0f;
    update_coefs();
}

void LowPassFilter::update_coefs() {
    float w0 = 2.0f * M_PI * cutoffFreq / sampleRate;
    float cosw0 = std::cos(w0);
    float alpha = std::sin(w0) / (2.0f * resonance);

    b0 = (1.0f - cosw0) / 2.0f;
    b1 = 1.0f - cosw0;
    b2 = (1.0f - cosw0) / 2.0f;
    a0 = 1.0f + alpha;
    a1 = -2.0f * cosw0;
    a2 = 1.0f - alpha;
}


float LowPassFilter::process(float input) {
    float output = b0 / a0 * input + b1 / a0 * x1 + b2 / a0 * x2
                     - a1 / a0 * y1 - a2 / a0 * y2;

    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = output;

    return output;
}

HighPassFilter::HighPassFilter(float sample_rate, float cutoffFrequency, float q) {
    sampleRate = sample_rate;
    cutoffFreq = cutoffFrequency;
    resonance = q;
    x1 = x2 = y1 = y2 = 0.0f;
    update_coefs();
}

void HighPassFilter::update_coefs() {
    float w0 = 2.0f * M_PI * cutoffFreq / sampleRate;
    float alpha = sin(w0) / (2.0f * resonance);

    b0 = (1.0f + cos(w0)) / 2.0f;
    b1 = -(1.0f + cos(w0));
    b2 = (1.0f + cos(w0)) / 2.0f;
    a0 = 1.0f + alpha;
    a1 = -2.0f * cos(w0);
    a2 = 1.0f - alpha;
}


float HighPassFilter::process(float input) {
    float output = (b0 / a0) * input + (b1 / a0) * x1 + (b2 / a0) * x2 - (a1 / a0) * y1 - (a2 / a0) * y2;

    // Update state variables
    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = output;

    // Return filtered output
    return output;
}