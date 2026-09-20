/* Two header layouts for the lease block, and the give-away sequence run
 * against both. The question: does a TRAILING header make the case the ffi
 * seat measured CORRECT, or only make its failure quieter?
 *
 * The sequence is the one route A makes expressible: the program leases, C
 * takes the pointer and frees it with a real destructor, and the program then
 * pays the end_lease every lease owes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct { uint64_t magic; int64_t len; } Header;
#define MAGIC UINT64_C(0x4845524f48454c44)

/* -- leading, which is the runtime today -------------------------------- */
static const char *lead_hold(const char *bytes, int64_t n) {
    Header *h = malloc(sizeof(Header) + (size_t)n + 1);
    h->magic = MAGIC; h->len = n;
    char *b = (char *)(h + 1);
    memcpy(b, bytes, (size_t)n); b[n] = '\0';
    return b;
}
/* What the runtime does on a bad magic word: hero_panic, which aborts, and the
 * message blames the COMPILER — `runtime/parts/str.c:462-466`. */
static void panic_like_the_runtime(void) {
    fprintf(stderr, "panic: end_lease of a pointer no `.lease()` made — the checker "
                    "admits `end_lease` only on a lease cell, so this is a compiler "
                    "bug, please report it\n");
    abort();
}

static void lead_release(const char **slot) {
    const char *p = *slot;
    Header *h = (Header *)(void *)((char *)(uintptr_t)p - sizeof(Header));
    if (h->magic != MAGIC) panic_like_the_runtime();
    h->magic = 0; *slot = NULL; free(h);
}

/* -- trailing, which panel 167 adopted ---------------------------------- */
static size_t pad(int64_t n) { /* the bytes, the NUL, then the header aligned */
    size_t raw = (size_t)n + 1;
    size_t a = _Alignof(Header);
    return (raw + a - 1) / a * a;
}
static const char *trail_hold(const char *bytes, int64_t n) {
    char *b = malloc(pad(n) + sizeof(Header));
    memcpy(b, bytes, (size_t)n); b[n] = '\0';
    Header *h = (Header *)(void *)(b + pad(n));
    h->magic = MAGIC; h->len = n;
    return b;
}
static void trail_release_n(const char **slot, int64_t n) {
    const char *p = *slot;
    Header *h = (Header *)(void *)((char *)(uintptr_t)p + pad(n));
    if (h->magic != MAGIC) panic_like_the_runtime();
    h->magic = 0; *slot = NULL; free((void *)(uintptr_t)p);
}

/* C's side: a real destructor, sqlite3_bind_blob's fifth argument. */
static void c_destructor(void *p) { free(p); }

int main(int argc, char **argv) {
    const char *src = "\x0A\x14\x1E\x28\x32\x3C\x46\x50";
    int64_t n = 8;
    int which = argc > 1 ? atoi(argv[1]) : 0;

    if (which == 0) {
        printf("A leading  + C frees + end_lease\n");
        const char *p = lead_hold(src, n);
        c_destructor((void *)(uintptr_t)p);   /* what sqlite3_free does */
        lead_release(&p);
        printf("  survived\n");
    } else if (which == 1) {
        printf("B trailing + C frees + end_lease\n");
        const char *p = trail_hold(src, n);
        c_destructor((void *)(uintptr_t)p);
        trail_release_n(&p, n);
        printf("  survived\n");
    } else if (which == 2) {
        printf("C leading  + no C free + end_lease (the ordinary case)\n");
        const char *p = lead_hold(src, n);
        printf("  C reads %02X%02X\n", (unsigned char)p[0], (unsigned char)p[7]);
        lead_release(&p);
        printf("  survived\n");
    } else {
        printf("D trailing + no C free + end_lease with the length\n");
        const char *p = trail_hold(src, n);
        printf("  C reads %02X%02X\n", (unsigned char)p[0], (unsigned char)p[7]);
        trail_release_n(&p, n);
        printf("  survived\n");
    }
    return 0;
}
