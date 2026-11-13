/* Defect 2520, 2026-11-13: a struct whose one member the header names as
 * glibc names `sigset_t`'s, beside its cases so every platform reads it. */
typedef struct {
    unsigned long __val[16];
} set2520_t;
