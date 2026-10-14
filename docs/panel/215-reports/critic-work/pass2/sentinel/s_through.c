/* Critic, panel 215 pass 2: the callee, in its own translation unit. It
   counts the padding bytes of the record it received by value that are not
   the caller's zeros, then hands the record back by value. */
#include <stdio.h>
struct cd { char c; double d; };
struct cd through(struct cd x) {
    const unsigned char *b = (const unsigned char *)&x;
    int n = 0;
    for (int i = 1; i < 8; i++) n += b[i] != 0;
    printf("callee: %d of 7 padding bytes nonzero\n", n);
    return x;
}
