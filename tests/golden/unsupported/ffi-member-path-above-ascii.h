/* Found beside defect 2508, 2026-11-13: a field C reaches only inside two
 * members whose names are above ASCII. */
struct deep_above { int kind; struct { union { int fd; void *h; } 結; } 内; };
