/* Beside defect 156's cases, lane land186, 2026-10-02 (panel 186 R5): a
   bit-field where a record can meet one, named, left out, at the end, inside
   an anonymous struct and as wide as its type, so no case leans on a system
   header. */
#include <stdint.h>

/* the critic's own struct */
typedef struct { int32_t kind; uint32_t flag : 1; uint32_t rest : 31; } BF;

/* a bit-field as the last member */
typedef struct { int32_t a; uint32_t last : 1; } ATEND;

/* bit-fields inside an anonymous struct */
typedef struct { int32_t kind; struct { uint8_t lo : 4; uint8_t hi : 4; }; } INANON;

/* a bit-field as wide as its type, still a bit-field */
typedef struct { int32_t kind; uint32_t whole : 32; } FULL;
