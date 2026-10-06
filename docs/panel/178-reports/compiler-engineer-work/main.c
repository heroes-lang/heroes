#include <stdio.h>
#include "q.h"
int main(void) {
    /* c = -1: an int8_t whose register copy is sign-extended */
    struct q a = make_literal(-1, 7);
    struct q b = make_memset(-1, 7);
    printf("literal shape: nonzero padding bytes %d of 3\n", count_nonzero_padding(&a));
    printf("memset shape:  nonzero padding bytes %d of 3\n", count_nonzero_padding(&b));
    return 0;
}
