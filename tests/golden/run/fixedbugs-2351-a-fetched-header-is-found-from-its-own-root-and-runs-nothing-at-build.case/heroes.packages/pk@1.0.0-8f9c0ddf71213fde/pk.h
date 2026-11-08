/* The package `pk`'s own header, placed beside its module by `heroes fetch`
 * (defect 2351). Its constructor appends a line to a file of the build's own
 * scratch wherever its code runs, and `pk_witnessed` counts those lines and
 * takes the file away: a build that runs the header's code leaves lines the
 * program then counts. */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define PK_WITNESS "build/fixedbugs-2351-witness.txt"
#define PK_LIMIT ((1LL << 40) + 3)
#define PK_TOP UINT64_MAX
#define PK_ON ((_Bool)1)

static inline int64_t pk_twice(int64_t x) { return x * 2; }

__attribute__((constructor)) static void pk_ran(void) {
    FILE *f = fopen(PK_WITNESS, "a");
    if (f) {
        fputs("ran\n", f);
        fclose(f);
    }
}

static inline int64_t pk_witnessed(void) {
    FILE *f = fopen(PK_WITNESS, "r");
    if (!f) {
        return 0;
    }
    int64_t lines = 0;
    int ch;
    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }
    fclose(f);
    remove(PK_WITNESS);
    return lines;
}
