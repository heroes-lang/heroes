/* The header of a program holding a float's `%` to C's own `fmod`: a double
 * from its bits and back, and the library's answer, which the program links
 * libm for, as a program calling `fmod` itself must. */
#include <math.h>
#include <stdint.h>
#include <string.h>

static inline double rem_case_of_bits(int64_t bits) {
    double x;
    memcpy(&x, &bits, sizeof x);
    return x;
}

static inline int64_t rem_case_bits(double x) {
    int64_t bits;
    memcpy(&bits, &x, sizeof bits);
    return bits;
}

static inline double rem_case_fmod(double x, double y) { return fmod(x, y); }
