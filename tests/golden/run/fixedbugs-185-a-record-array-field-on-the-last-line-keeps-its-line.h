/* Defect 185's third shape: an array of another struct, the last member of
   the last record its group declares. */
#include <stdint.h>

typedef struct {
    int32_t v;
} Cell;

typedef struct {
    int32_t count;
    Cell cells[2];
} Holder;

static inline int32_t total(Holder h) { return h.cells[0].v + h.cells[1].v; }
