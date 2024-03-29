#ifndef UTILS_H
#define UTILS_H

#define PI 3.14159265
#define TWO_PI PI * 2.0
#define TWO_PI_RECIP 1.0 / TWO_PI
#define END_OF_CYCLE TWO_PI - 0.0001
#define START_OF_CYCLE 0.001

inline float mtof(int note) {
    return 440.0 * powf(2.0, (( (float)(note-69) )/12.0));
}

#endif