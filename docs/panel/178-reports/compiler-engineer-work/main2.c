#include <stdio.h>
#include <string.h>
#include "q.h"
void shape_control(struct q *, int8_t, int32_t);
void shape_literal(struct q *, int8_t, int32_t);
void shape_omitted(struct q *, int32_t);
void shape_zero_then_store(struct q *, int8_t, int32_t);
void shape_memset(struct q *, int8_t, int32_t);
void shape_memset_in_place(struct q *, int8_t, int32_t);
static int aa(const struct q *p) { const unsigned char *b = (const unsigned char *)p; int k = 0; for (int n = 1; n < 4; n++) k += b[n] == 0xAA; return k; }
int main(void) {
    struct q d;
    memset(&d, 0xAA, sizeof d); shape_control(&d, -1, 7);         printf("control            0xAA padding left: %d of 3\n", aa(&d));
    memset(&d, 0xAA, sizeof d); shape_literal(&d, -1, 7);         printf("literal (today)    0xAA padding left: %d of 3\n", aa(&d));
    memset(&d, 0xAA, sizeof d); shape_omitted(&d, 7);             printf("omitted field (R1) 0xAA padding left: %d of 3\n", aa(&d));
    memset(&d, 0xAA, sizeof d); shape_zero_then_store(&d, -1, 7); printf("zero then store    0xAA padding left: %d of 3\n", aa(&d));
    memset(&d, 0xAA, sizeof d); shape_memset(&d, -1, 7);          printf("memset then store  0xAA padding left: %d of 3\n", aa(&d));
    memset(&d, 0xAA, sizeof d); shape_memset_in_place(&d, -1, 7); printf("memset in place    0xAA padding left: %d of 3\n", aa(&d));
    return 0;
}
