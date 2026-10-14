#include <stdio.h>
#include <string.h>
#include <stdint.h>
struct cd { char c; double d; };
int main(void) {
    struct cd x; memset(&x, 0xAA, sizeof x); x.c = 1; x.d = 2.0;
    __builtin_clear_padding(&x);
    const unsigned char *b = (const unsigned char *)&x;
    for (size_t i = 0; i < sizeof x; i++) printf("%02x", b[i]);
    printf("\n");
    return 0;
}
