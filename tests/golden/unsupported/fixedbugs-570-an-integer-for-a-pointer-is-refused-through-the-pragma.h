/* Defect 570, 2026-10-10: an integer for a pointer behind an ignored pragma. */
#pragma clang diagnostic ignored "-Wint-conversion"
static inline int read_p(int *p) { return *p; }
