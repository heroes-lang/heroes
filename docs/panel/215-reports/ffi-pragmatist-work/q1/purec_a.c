/* ffi-pragmatist, panel 215: pure C, no Heroes. A record zeroed whole, then passed and returned by
   value through a function in ANOTHER translation unit, then written as bytes. */
#include <string.h>
#include <unistd.h>
struct cd { char c; double d; };
struct cd pass_cd(struct cd x);               /* purec_b.c */
long take_cd(struct cd x);                    /* purec_b.c: C itself takes the record by value */
int main(void) {
    struct cd a; memset(&a, 0, sizeof a); a.c = 1; a.d = 2.0;   /* CERT DCL39-C's first remedy */
    long n = write(1, &a, sizeof a);                            /* line 10: clean expected */
    struct cd b = pass_cd(a);
    n += write(1, &b, sizeof b);                                /* line 12: through a call by value */
    n += take_cd(a);                                            /* C's own by-value parameter */
    struct cd z; memset(&z, 0, sizeof z); z.c = 1; z.d = 2.0;
    struct cd *zp = &z; long m = write(1, zp, sizeof *zp);      /* line 15: by address, clean expected */
    return (int)((n + m) & 0);
}
