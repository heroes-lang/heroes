#include "q.h"
int count_nonzero_padding(const struct q *p) { const unsigned char *b = (const unsigned char *)p; int k = 0; for (size_t n = 1; n < 4; n++) k += b[n] != 0; return k; }
