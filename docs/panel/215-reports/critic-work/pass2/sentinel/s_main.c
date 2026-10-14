/* Critic, panel 215 pass 2: a sentinel probe, no sanitiser. The caller zeroes
   the whole record (CERT DCL39-C's first remedy), fills the stack below it
   with 0xAA, passes the record by value to another unit and gets it back by
   value into a slot it also filled with 0xAA. A nonzero padding byte on
   either side is a byte the call did not carry. */
#include <stdio.h>
#include <string.h>
struct cd { char c; double d; };
struct cd through(struct cd x);
__attribute__((noinline)) static void dirty(void) {
    volatile unsigned char buf[4096];
    for (int i = 0; i < 4096; i++) buf[i] = 0xAA;
}
int main(void) {
    struct cd a, r;
    memset(&a, 0, sizeof a); a.c = 1; a.d = 2.0;
    memset(&r, 0xAA, sizeof r);
    dirty();
    r = through(a);
    const unsigned char *b = (const unsigned char *)&r;
    int n = 0;
    for (int i = 1; i < 8; i++) n += b[i] != 0;
    printf("caller: %d of 7 padding bytes nonzero after the return\n", n);
    return 0;
}
